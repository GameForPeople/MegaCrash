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
    float3 boneWeights : WEIGHTS;
    uint4 boneIndices : BONEINDICES;
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

    float fWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    fWeights[0] = input.boneWeights.x;
    fWeights[1] = input.boneWeights.y;
    fWeights[2] = input.boneWeights.z;
    fWeights[3] = 1.0f - fWeights[0] - fWeights[1] - fWeights[2];

    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    float3 tangent = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; ++i)
    {
        pos += fWeights[i] * mul(float4(input.position, 1.0f), gmtxAnimationTransforms[input.boneIndices[i]]).xyz;
        normal += fWeights[i] * mul(input.normal, (float3x3) gmtxAnimationTransforms[input.boneIndices[i]]);
        tangent += fWeights[i] * mul(input.tangent, (float3x3) gmtxAnimationTransforms[input.boneIndices[i]]);
    }

    output.normalW = mul(normal, (float3x3) gmtxGameObject);
    output.positionW = (float3) mul(float4(pos, 1.0f), gmtxGameObject);
    output.ShadowPos = mul(float4(output.positionW, 1.0f), gmtxVPT);
    output.tangentW = mul(tangent, (float3x3) gmtxGameObject);
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.uv = input.uv;
    output.material = input.material;
    return (output);
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float3 normal = float3(gtxtModelNormalMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));

    // 림라이트 구현. pow 값을 높여 테두리의 두께를 얇게 만들 수 있다.
    float4 cEmissiveColor = gtxtEmissiveMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    if (cEmissiveColor.a > 0.1f)
    {
        float rim = dot(normalW, normalize(gvCameraPosition - input.positionW));
        return saturate(cEmissiveColor * (1 + pow(1 - rim, 2)));
    }
    
    float4 cDiffuseColor = gtxtModelTexture[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    cDiffuseColor *= (1.0f - cSpecularColor);

    // r채널만 추출한다.
    float cGlossiness = gtxtGlossinessMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).r;

    //----------------<Get Reflection Color>---------------------------------

    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vToCamera = normalize(vCameraPosition - input.positionW);
    float3 r = reflect(-vToCamera, input.normalW);
    float4 reflectionColor = gtxtSkyBox.Sample(gLinearWrapSampler, r);

    //-----------------------------------------------------------


    float4 ResultColor = BRDFLighting(
          input.positionW
        , normalW
        , cDiffuseColor
        , cSpecularColor
        , cGlossiness
        , reflectionColor
        , CalcShadowFactor(input.ShadowPos));

    return ResultColor;
}

PS_DEFFERED_OUTPUT PS_DEFFERED(VS_OUTPUT input)
{
    PS_DEFFERED_OUTPUT output;
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float3 normal = float3(gtxtModelNormalMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));

    // 림라이트 구현. pow 값을 높여 테두리의 두께를 얇게 만들 수 있다.
    float4 cEmissiveColor = gtxtEmissiveMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    if (cEmissiveColor.a > 0.1f)
    {
        float rim = dot(normalW, normalize(gvCameraPosition - input.positionW));
        cEmissiveColor = saturate(cEmissiveColor * (1 + pow(1 - rim, 2)));
    }
    
    float4 cDiffuseColor = gtxtModelTexture[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv);
    cDiffuseColor.rgb *= (1.0f - cSpecularColor.rgb);

    output.posW_bActive = float4(input.positionW, 1);
    output.normW_PostRimType.xyz = normalW;
    output.normW_PostRimType.w = POST_RIM_TYPE_NONE;
    output.ShadowposW = input.ShadowPos;
    output.diffuse = cDiffuseColor;
    output.SpecC_GlossF.rgb = cSpecularColor;
    output.SpecC_GlossF.w = gtxtGlossinessMap[NonUniformResourceIndex(input.material)].Sample(gLinearWrapSampler, input.uv).r;
    output.emissive = cEmissiveColor;

    return output;
}