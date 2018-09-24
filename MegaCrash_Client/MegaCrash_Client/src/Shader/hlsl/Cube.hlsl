#include "Common.hlsl"
#include "Shadow.hlsl"

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION0;
    float4 ShadowPos : POSITION1;
    float3 normalW : NORMAL;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output;
    output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;
    output.ShadowPos = mul(float4(output.positionW, 1.0f), gmtxVPT);
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxGameObject);

    return (output);
}
float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    // 림라이트 구현
    float rim = saturate(dot(input.normalW, normalize(gvCameraPosition - input.positionW)));

   //----------------<Get Reflection Color>---------------------------------

    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vToCamera = normalize(vCameraPosition - input.positionW);
    float3 r = reflect(-vToCamera, input.normalW);
    float4 reflectionColor = gtxtSkyBox.Sample(gLinearWrapSampler, r);

    //-----------------------------------------------------------

    float4 LightingResult = BRDFLighting(
          input.positionW
        , input.normalW
        , gMaterials[gnMaterial].m_cDiffuse
        , gMaterials[gnMaterial].m_cSpecular
        , gMaterials[gnMaterial].m_cSpecular.a
        , reflectionColor
        , CalcShadowFactor(input.ShadowPos));

    float4 cColor = LightingResult * 0.7 + LightingResult * pow(rim, 5) * 3;
    cColor += lerp(cColor, reflectionColor * CalcShadowFactor(input.ShadowPos), 0.2);
    return saturate(cColor + gMaterials[gnMaterial].m_cEmissive);
}

PS_DEFFERED_OUTPUT PS_DEFFERED(VS_OUTPUT input)
{
    PS_DEFFERED_OUTPUT output;
    output.posW_bActive = float4(input.positionW, 1);
    output.normW_PostRimType.xyz = input.normalW;
    output.normW_PostRimType.w = POST_RIM_TYPE_CUBE;
    output.ShadowposW = input.ShadowPos;
    output.diffuse = gMaterials[gnMaterial].m_cDiffuse;
    output.SpecC_GlossF = gMaterials[gnMaterial].m_cSpecular;
    output.emissive = gMaterials[gnMaterial].m_cEmissive;
    return output;
}