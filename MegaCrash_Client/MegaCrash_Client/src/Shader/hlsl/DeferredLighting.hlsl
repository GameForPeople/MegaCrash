#include "Common.hlsl"
#include "Shadow.hlsl"

Texture2D<float4> gtxtDiffuse                : register(t2);
Texture2D<float4> gtxtPosW_bActive           : register(t3);
Texture2D<float4> gtxtShadowposW             : register(t4);
Texture2D<float4> gtxtNormW_PostRimType      : register(t5);
Texture2D<float4> gtxtSpecC_GlossF           : register(t6);
Texture2D<float4> gtxtEmissive               : register(t7);

float4 VS_MAIN(uint nVertexID : SV_VertexID) : SV_POSITION
{
    if (nVertexID == 0)
        return (float4(-1.0f, +1.0f, 0.0f, 1.0f));
    if (nVertexID == 1)
        return (float4(+1.0f, +1.0f, 0.0f, 1.0f));
    if (nVertexID == 2)
        return (float4(+1.0f, -1.0f, 0.0f, 1.0f));
    if (nVertexID == 3)
        return (float4(-1.0f, +1.0f, 0.0f, 1.0f));
    if (nVertexID == 4)
        return (float4(+1.0f, -1.0f, 0.0f, 1.0f));
    if (nVertexID == 5)
        return (float4(-1.0f, -1.0f, 0.0f, 1.0f));
    return (float4(0, 0, 0, 0));
}
static const int filter_size = 81;
static const float filter_factor = 1.0f / float(filter_size);
static int2 gnOffsets[filter_size] = { 
          { -4, -4 }, { -3, -4 }, { -2, -4 }, { 1, -4 }, { 0, -4 }, { 1, -4 }, { 2, -4 }, { 3, -4 }, { 4, -4 }
        , { -4, -3 }, { -3, -3 }, { -2, -3 }, { 1, -3 }, { 0, -3 }, { 1, -3 }, { 2, -3 }, { 3, -3 }, { 4, -3 }
        , { -4, -2 }, { -3, -2 }, { -2, -2 }, { 1, -2 }, { 0, -2 }, { 1, -2 }, { 2, -2 }, { 3, -2 }, { 4, -2 }
        , { -4, -1 }, { -3, -1 }, { -2, -1 }, { 1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 }, { 3, -1 }, { 4, -1 }
        , { -4, -0 }, { -3, -0 }, { -2, -0 }, { 1, -0 }, { 0, -0 }, { 1, -0 }, { 2, -0 }, { 3, -0 }, { 4, -0 }
        , { -4,  1 }, { -3,  1 }, { -2,  1 }, { 1,  1 }, { 0,  1 }, { 1,  1 }, { 2,  1 }, { 3,  1 }, { 4,  1 }
        , { -4,  2 }, { -3,  2 }, { -2,  2 }, { 1,  2 }, { 0,  2 }, { 1,  2 }, { 2,  2 }, { 3,  2 }, { 4,  2 }
        , { -4,  3 }, { -3,  3 }, { -2,  3 }, { 1,  3 }, { 0,  3 }, { 1,  3 }, { 2,  3 }, { 3,  3 }, { 4,  3 }
        , { -4,  4 }, { -3,  4 }, { -2,  4 }, { 1,  4 }, { 0,  4 }, { 1,  4 }, { 2,  4 }, { 3,  4 }, { 4,  4 } };

float4 PS_MAIN(float4 pos : SV_POSITION) : SV_Target
{
    int2 uv = int2(pos.xy);
    float4 diffuse = gtxtDiffuse[uv];
    float4 posW_bActive = gtxtPosW_bActive[uv];
    if (posW_bActive.w > 0.9f)
    {
        float3 posW = posW_bActive.xyz;
        float3 normW = gtxtNormW_PostRimType[uv].xyz;
        float PostRimType = gtxtNormW_PostRimType[uv].w;
        float4 ShadowPosW = gtxtShadowposW[uv];
        float ShadowFactor = CalcShadowFactor(ShadowPosW);
        float4 Specular = float4(gtxtSpecC_GlossF[uv].rgb, 1);
        float Glossiness = gtxtSpecC_GlossF[uv].a;
        float4 Emissive = gtxtEmissive[uv];

        //return float4(ShadowFactor, ShadowFactor, ShadowFactor, 1);

        float3 color = float3(0, 0, 0);
        for (int i = 0; i < filter_size; i++)
        {
            color += filter_factor * gtxtEmissive[uv + gnOffsets[i]].xyz;
        }
        float4 BluredEmissive = float4(color, 1);

    //--------------------<Get Reflection Color>-------------------------

        float3 vCameraPosition = gvCameraPosition.xyz;
        float3 vToCamera = normalize(vCameraPosition - posW);
        float3 r = reflect(-vToCamera, normW);
        float4 reflectionColor = gtxtSkyBox.Sample(gLinearWrapSampler, r);

    //-------------------------------------------------------------------

        float4 ResultColor = BRDFLighting(
              posW
            , normW
            , diffuse
            , Specular
            , Glossiness
            , reflectionColor
            , ShadowFactor);

        if (PostRimType > POST_RIM_TYPE_STAGE - 0.01f)
        {
            float rim = saturate(dot(normW, normalize(gvCameraPosition - posW)));
            ResultColor = ResultColor * 0.5 + ResultColor * pow(rim, 7) * 0.5;
            ResultColor += reflectionColor * ShadowFactor * 0.5;
            ResultColor.rgb += Emissive.rgb * Emissive.a;
            return float4(saturate(ResultColor).rgb, 1);
        }
        else if (PostRimType >= POST_RIM_TYPE_CUBE - 0.01f)
        {
            float rim = saturate(dot(normW, normalize(gvCameraPosition - posW)));
            ResultColor = ResultColor * 0.5 + ResultColor * pow(rim, 7) * 0.5;
            ResultColor += reflectionColor * ShadowFactor * 0.5;
            ResultColor.rgb += Emissive.rgb * Emissive.a;
            ResultColor = float4(saturate(ResultColor).rgb, 1);
            //float rim = saturate(dot(normW, normalize(gvCameraPosition - posW)));
            //float4 cColor = ResultColor * 0.7 + ResultColor * pow(rim, 7) * 3;
            //cColor += lerp(cColor, reflectionColor * ShadowFactor, 0.2);
            //ResultColor = float4(saturate(cColor + Emissive).rgb, diffuse.a);
        }
        else if (Emissive.a > 0.1f)
            ResultColor = float4(Emissive.rgb, 1);
        else
            ResultColor = float4(saturate(ResultColor).rgb, 1);

        return saturate(ResultColor + BluredEmissive);
        //return saturate(BluredEmissive);
    }
    else
    {
        return diffuse;
    }
}