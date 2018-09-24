#include "Common.hlsl"
#include "Shadow.hlsl"

Texture2D gtxtModelTexture[10] : register(t2);
Texture2D gtxtModelNormalMap[10] : register(t12);
Texture2D gtxtSpecularMap[10] : register(t22);
Texture2D gtxtGlossinessMap[10] : register(t32);
Texture2D gtxtEmissiveMap[10] : register(t42);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    uint material : MATERIAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION0;
    float4 ShadowPos : POSITION1;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 uv : TEXCOORD;
    uint material : MATERIAL;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;
    output.ShadowPos = mul(float4(output.positionW, 1.0f), gmtxVPT);
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxGameObject);
    output.tangentW = mul(input.tangent, (float3x3) gmtxGameObject);
    output.uv = input.uv;
    output.material = input.material;

    return (output);
}

float3 Circuit(float2 uv, float timer, float distance_delta, float color_distance_delta, bool isCircle)
{
    // 깜박임 효과를 내기 위함
    float flicker = noise1D(timer * 2.0) * 0.8;

    // 입력된 UV에 배수를 추가로 줄 수록 촘촘해진다.

    uv *= (isCircle) ? 0.01 * (distance_delta ) : 2;

    float v = 0.0;
	
	// 회로망의 밝기를 결정한다.
    float a = 0.6;
    // 다음 루프에서의 오프셋을 결정한다. 
    //( 위치 이동, 회로망이 같은 위치에, 같은 스케일로 그려지지 않게 )
    float f = 3.0;
	
    // 루프가 많아질수록 더 촘촘한 패턴이 만들어진다.
    // 추가할 수록 느려지니 주의
    int loop_factor = (isCircle) ? 5 : 2;


    for (int i = 0; i < loop_factor; i++) 
    {
        float v1 = voronoi( uv * f);
        float v2 = 0.0;
		
		// 추가 루프에서 회로망을 따라 움직이는 전자효과 만들기 
        if (i > 0)
        {
			// 보로노이 방정식을 기반으로 회로망 생성, 
            //( 안하면 하예짐, 어디를 어둡게 만들지를 결정한다 보면 된다)
            v2 = voronoi( uv * f * 0.5 + timer);
			
            float va = 0.0;
            float vb = 0.0;

            va = 1.0 - smoothstep(0.0, 0.1, v1);
            // 최대 값이 커질수록 전자 이동 선이 두꺼워짐
            vb = 1.0 - smoothstep(0.0, 0.08, v2);
            // 전자가 흐르는 부분에 색상을 가산한다.
            v += a * pow(va * (0.5 + vb), 2.0);
        }
		
		// 선명한 가장자리 선 만들기
        v1 = 1.0 - smoothstep(0.0, 0.3, v1);
		
		// 회로의 선이 그어진 위치에만 색상이 입혀지도록 한다.
        // v1 에 배수가 높아질 수록 패턴 내부 무늬가 더 많이 새겨진다. ( 사인 주기를 생각 )
        // 3.14 가 곱해진 사인 곡선을 생각해보자.
        v2 = a * noise1D(v1 * 5.0 + 0.1);
		
		// 1차 회로망은 깜빡이게 만들자
        if (i == 0)
            v += v2 * flicker;
        else
            v += v2;
		
        f *= 2.0;

        // 루프가 반복될 수록 점점 회로망이 어두워진다.
        a *= 0.6;
    }

    float3 cexp = float3(0,0,0);

    float3 color_01 = float3(6.0, 1.0, 1.0);
    float3 color_02 = float3(1.0, 1.0, 6.0);

    cexp = lerp(color_01, color_02, color_distance_delta);
	
    float3 col = float3(pow(v, cexp.x), pow(v, cexp.y), pow(v, cexp.z)) * 2.0;
	
    return col;


}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float rim = saturate(dot(input.normalW, normalize(gvCameraPosition - input.positionW)));

    float3 normal = float3(gtxtModelNormalMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).xyz);
    //float3 normal = float3(gtxtModelNormalMap[0].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));
    
    float4 cDiffuseColor = gtxtModelTexture[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    float4 cGlossiness = gtxtGlossinessMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    // 서킷 텍스쳐 하나만 사용
    float4 cEmissiveColor = gtxtEmissiveMap[NonUniformResourceIndex(0)].Sample(gLinearWrapSampler, input.uv*8.0);
    // 0 ~ 50000 거리 내 벽 표면에 회로가 출력됨
    float circuit_max_Distance = 50000;
    // 원형 회로 최대 거리
    float unit_circle_max_Distance = 170;

    float circuit_delta = InverseLerp(0, circuit_max_Distance, clamp(distance(PlayerPosition.xyz, input.positionW), 0, circuit_max_Distance));
    float unit_circuit_dist_delta = InverseLerp(0, unit_circle_max_Distance, clamp(distance(PlayerPosition.xyz, input.positionW), 0, unit_circle_max_Distance));
    float circuit_Color_delta = InverseLerp(0, circuit_max_Distance * 0.1, clamp(distance(PlayerPosition.xyz, input.positionW), 0, circuit_max_Distance * 0.1));
    
    cEmissiveColor *= ( 1 - circuit_delta) * cSpecularColor;
    cEmissiveColor += cEmissiveColor * pow(1-rim,5);

    cEmissiveColor.rgb *= Circuit(input.uv, gfTimer, circuit_delta, circuit_Color_delta, false) * 2;

    cEmissiveColor.rgb +=
    Circuit(input.uv, gfTimer, unit_circuit_dist_delta, circuit_Color_delta, true)
    * (1 - unit_circuit_dist_delta)*2;

    cEmissiveColor.rgb = lerp(cEmissiveColor.rgb, CalcShadowFactor(input.ShadowPos) * cEmissiveColor.rgb, 0.5f);

    //----------------<Get Reflection Color>---------------------------------

    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vToCamera = normalize(vCameraPosition - input.positionW);
    float3 r = reflect(-vToCamera, normalW);
    float4 reflectionColor = gtxtSkyBox.Sample(gLinearWrapSampler, r);

    //-----------------------------------------------------------


    float4 ResultColor = BRDFLighting(
          input.positionW
        , normalW
        , cDiffuseColor
        , cSpecularColor + cEmissiveColor
        , 1
        , reflectionColor
        , CalcShadowFactor(input.ShadowPos));

    ResultColor = ResultColor * 0.5 + ResultColor * pow(rim, 7) * 0.5;
    ResultColor += reflectionColor * ( CalcShadowFactor(input.ShadowPos))*0.5;
    ResultColor.rgb += cEmissiveColor.rgb * cEmissiveColor.a ;
    return saturate(ResultColor);
}

PS_DEFFERED_OUTPUT PS_DEFFERED(VS_OUTPUT input)
{
    PS_DEFFERED_OUTPUT output;
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float rim = saturate(dot(input.normalW, normalize(gvCameraPosition - input.positionW)));

    float3 normal = float3(gtxtModelNormalMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).xyz);
    //float3 normal = float3(gtxtModelNormalMap[0].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));
    
    float4 cDiffuseColor = gtxtModelTexture[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    // 서킷 텍스쳐 하나만 사용
    float4 cEmissiveColor = gtxtEmissiveMap[NonUniformResourceIndex(0)].Sample(gLinearWrapSampler, input.uv * 8.0);
    // 0 ~ 50000 거리 내 벽 표면에 회로가 출력됨
    float circuit_max_Distance = 50000;
    // 원형 회로 최대 거리
    float unit_circle_max_Distance = 170;

    float circuit_delta = InverseLerp(0, circuit_max_Distance, clamp(distance(PlayerPosition.xyz, input.positionW), 0, circuit_max_Distance));
    float unit_circuit_dist_delta = InverseLerp(0, unit_circle_max_Distance, clamp(distance(PlayerPosition.xyz, input.positionW), 0, unit_circle_max_Distance));
    float circuit_Color_delta = InverseLerp(0, circuit_max_Distance * 0.1, clamp(distance(PlayerPosition.xyz, input.positionW), 0, circuit_max_Distance * 0.1));
    
    cEmissiveColor *= (1 - circuit_delta) * cSpecularColor;
    cEmissiveColor += cEmissiveColor * pow(1 - rim, 5);

    cEmissiveColor.rgb *= Circuit(input.uv, gfTimer, circuit_delta, circuit_Color_delta, false) * 2;

    cEmissiveColor.rgb +=
    Circuit(input.uv, gfTimer, unit_circuit_dist_delta, circuit_Color_delta, true)
    * (1 - unit_circuit_dist_delta) * 2;

    cEmissiveColor.rgb = lerp(cEmissiveColor.rgb, CalcShadowFactor(input.ShadowPos) * cEmissiveColor.rgb, 0.5f);

    output.posW_bActive = float4(input.positionW, 1);
    output.normW_PostRimType.xyz = normalW;
    output.normW_PostRimType.w = POST_RIM_TYPE_STAGE;
    output.ShadowposW = input.ShadowPos;
    output.diffuse = cDiffuseColor;
    output.SpecC_GlossF.rgb = (cSpecularColor + cEmissiveColor).rgb;
    output.SpecC_GlossF.w = 1;
    output.emissive = cEmissiveColor;

    return output;
}