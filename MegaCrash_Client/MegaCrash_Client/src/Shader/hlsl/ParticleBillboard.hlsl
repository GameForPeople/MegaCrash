#include "Common.hlsl"

Texture2D gtxtParticle : register(t2);

struct VS_INPUT
{
    float3 posW : POSITION;
    float3 velocity : VELOCITY;
    float4 color : COLOR;
    float2 sizeW : SIZE;
    float3 option : OPTION; // isForcedByGravity , isStretchedBB, isAlignedByMovDir 
                                        // 옵션은 0.5 보다 큰가, 작은가로 판별한다.
    float4 value : VALUE; // SpinRate, SeedValue, CurrLife, GravityForce
    float4 customcolor : CUSTOMCOLOR; // 색상 추가 연산하고 싶을 때 
    float2 sizeMultiplier : SIZEMULTI; // 상대적 크기 조정 배수
};

struct GS_INPUT
{
    float3 centerW : POSITION;
    float2 sizeW : SIZE;
    float4 color : COLOR;
    float3 option : OPTION; // isForcedByGravity , isStretchedBB, isAlignedByMovDir 
                                        // 옵션은 0.5 보다 큰가, 작은가로 판별한다.
    float4 value : VALUE; // SpinRate, SeedValue, CurrLife, GravityForce
    float4 customcolor : CUSTOMCOLOR; // 색상 추가 연산하고 싶을 때 
    float2 sizeMultiplier : SIZEMULTI; // 상대적 크기 조정 배수
};

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
    float3 option : OPTION; // isForcedByGravity , isStretchedBB, isAlignedByMovDir 
                                        // 옵션은 0.5 보다 큰가, 작은가로 판별한다.
    float4 value : VALUE; // SpinRate, SeedValue, CurrLife, GravityForce
    float4 customcolor : CUSTOMCOLOR; // 색상 추가 연산하고 싶을 때 
};

GS_INPUT VS_MAIN(VS_INPUT input)
{
    GS_INPUT output;
    output.centerW = input.posW;

    // 운동 적용
    output.centerW += input.value.z * input.velocity;

   // 중력 적용
   if (input.option.x > 0)
   {
       output.centerW
       = output.centerW +
       0.5 * float3(0, -1, 0) * pow(input.value.z, 2) * input.value.w;
    }
    
    output.sizeW = input.sizeW;
    output.color = input.color;
    output.customcolor = input.customcolor;
    output.option = input.option;
    output.value = input.value;
    output.sizeMultiplier = input.sizeMultiplier;
    return output;
}

[maxvertexcount(4)]
void GS_MAIN(
	  point GS_INPUT input[1]
	, uint primID : SV_PrimitiveID
	, inout TriangleStream<PS_INPUT> outStream)
{

    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float3 vLook = gvCameraPosition.xyz - input[0].centerW;
    vLook = normalize(vLook);
    float3 vRight = cross(vUp, vLook);

    float fHalfW = input[0].sizeW.x * 0.5f * input[0].sizeMultiplier.x;
    float fHalfH = input[0].sizeW.y * 0.5f * input[0].sizeMultiplier.y;
    float3 vWidth = fHalfW * vRight;
    float3 vHeight = fHalfH * vUp;

    float4 pVertices[4] = {
          float4(input[0].centerW + vWidth - vHeight, 1.0f)
        , float4(input[0].centerW + vWidth + vHeight, 1.0f)
        , float4(input[0].centerW - vWidth - vHeight, 1.0f)
        , float4(input[0].centerW - vWidth + vHeight, 1.0f)
    };
    

    float2 pUVs[4] = {
          float2(-0.5, 0.5)
		, float2(-0.5, -0.5)
		, float2(0.5, 0.5)
		, float2(0.5, -0.5)
    };


    //float2 pUVs[4] =
    //{
    //    float2(-1, 1)
	//	, float2(-1, -1)
	//	, float2(1, 1)
	//	, float2(1, -1)
    //};


    // 회전 매트리스 계산
    float rotation_value = input[0].value.y + gfTimer * input[0].value.x;
    float s = sin(radians(rotation_value + input[0].value.y));
    float c = cos(radians(rotation_value + input[0].value.y));
    float2x2 rotationMatrix = float2x2(c, -s, s, c);
   
    PS_INPUT output;
    for (int i = 0; i < 4; i++)
    {
        output.posW = pVertices[i].xyz;
        output.posH = mul(pVertices[i], gmtxViewProjection);
        //output.posH = float4(pUVs[i].x, pUVs[i].y, 0,1);
        output.color = input[0].color;
        output.normalW = vLook;

        pUVs[i] = mul(pUVs[i], rotationMatrix);
        output.uv = pUVs[i]+0.5;

        output.option = input[0].option;
        output.value = input[0].value;
        output.customcolor = input[0].customcolor;
        outStream.Append(output);
    }
}

float4 PS_MAIN(PS_INPUT input) : SV_Target
{
    //return float4(1, 1, 1, 1);
    return gtxtParticle.Sample(gLinearClampSampler, input.uv) * input.color * input.customcolor;
}