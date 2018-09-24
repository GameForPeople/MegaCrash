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
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION0;
    float4 ShadowPos : POSITION1;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output;

    //float offset = gtxtModeldistanceMap.SampleLevel(gLinearWrapSampler, input.uv, 0).x;
    //input.position += input.normal * 10.f * (offset - 1.0f);

    output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;
    output.ShadowPos = mul(float4(output.positionW, 1.0f), gmtxVPT);
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxGameObject);
    output.tangentW = mul(input.tangent, (float3x3) gmtxGameObject);
    output.uv = input.uv;

    return (output);
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float4 cEmissiveColor = gtxtEmissiveMap[NonUniformResourceIndex(gnMaterial)].Sample(gLinearWrapSampler, input.uv);
    if (cEmissiveColor.a > 0.1f)
        return cEmissiveColor;

    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    float3 normal = float3(gtxtModelNormalMap[NonUniformResourceIndex(gnMaterial)].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));
    
    float4 cDiffuseColor = gtxtModelTexture[NonUniformResourceIndex(gnMaterial)].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[NonUniformResourceIndex(gnMaterial)].Sample(gLinearWrapSampler, input.uv);
    float4 cGlossiness = gtxtGlossinessMap[NonUniformResourceIndex(gnMaterial)].Sample(gLinearWrapSampler, input.uv);

    cDiffuseColor.rgb *= (1.0f - cSpecularColor.rgb);

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
        , cSpecularColor
        , cGlossiness.a
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
    
    float3 normal = float3(gtxtModelNormalMap[0].Sample(gLinearWrapSampler, input.uv).xyz);
    normal = 2.0f * normal - 1.0f;
    float3 normalW = normalize(mul(normal, TBN));
    
    float4 cDiffuseColor = gtxtModelTexture[0].Sample(gLinearWrapSampler, input.uv);
    float4 cSpecularColor = gtxtSpecularMap[0].Sample(gLinearWrapSampler, input.uv);
    cDiffuseColor *= (1.0f - cSpecularColor);

    output.posW_bActive = float4(input.positionW, 1);
    output.diffuse = cDiffuseColor;
    output.normW_PostRimType.xyz = normalW;
    output.normW_PostRimType.w = POST_RIM_TYPE_NONE;
    output.ShadowposW = input.ShadowPos;
    output.SpecC_GlossF.rgb = cSpecularColor;
    output.SpecC_GlossF.w = gtxtGlossinessMap[0].Sample(gLinearWrapSampler, input.uv).r;
    output.emissive = gtxtEmissiveMap[0].Sample(gLinearWrapSampler, input.uv);

    //output.diffuse            =float4 (0, 0, 0, 0);
    //output.posW_bActive       =float4 (0, 0, 0, 0);
    //output.ShadowposW         =float4 (0, 0, 0, 0);
    //output.normW_PostRimType  =float4 (0, 0, 0, 0);
    //output.SpecC_GlossF       =float4 (0, 0, 0, 0);
    //output.emissive = float4(0, 0, 0, 0);

    return output;
}