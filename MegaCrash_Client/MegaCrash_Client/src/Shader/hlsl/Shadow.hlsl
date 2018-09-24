#ifndef __SHADOW_HLSL__
#define __SHADOW_HLSL__

#include "Common.hlsl"

cbuffer cbShadowMap : register(b5)
{
    matrix gmtxVPT : packoffset(c0);
};
Texture2D gShadowMap : register(t1);
float CalcShadowFactor(float4 shadowPosH)
{
    //return gShadowMap.SampleCmpLevelZero(gShadowSampler, shadowPosH.xy, shadowPosH.z).r;
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;
    
    // Depth in NDC space.
    float depth = shadowPosH.z;
    
    uint width, height, numMips;
    gShadowMap.GetDimensions(0, width, height, numMips);
    
    // Texel size.
    float dx = 1.0f / float(width);
    
    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += gShadowMap.SampleCmpLevelZero(gShadowSampler, shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}

struct VS_STAGE_INPUT_SHADOW
{
    float3 Pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    uint material : MATERIAL;
};
struct VS_CUBE_INPUT_SHADOW
{
    float3 Pos : POSITION;
    float3 normal : NORMAL;
};
struct VS_MODEL_INPUT_SHADOW
{
    float3 Pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};
struct VS_ANIMATION_INPUT_SHADOW
{
    float3 Pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    float3 boneWeights : WEIGHTS;
    uint4 boneIndices : BONEINDICES;
    uint material : MATERIAL;
};

float4 VS_ShadowMap_Stage(VS_STAGE_INPUT_SHADOW input) : SV_POSITION
{
    return mul(mul(float4(input.Pos, 1.0f), gmtxGameObject), gmtxViewProjection);
}

float4 VS_ShadowMap_Cube(VS_CUBE_INPUT_SHADOW input) : SV_POSITION
{
    return mul(mul(float4(input.Pos, 1.0f), gmtxGameObject), gmtxViewProjection);
}

float4 VS_ShadowMap_Model(VS_MODEL_INPUT_SHADOW input) : SV_POSITION
{
    return mul(mul(float4(input.Pos, 1.0f), gmtxGameObject), gmtxViewProjection);
}

float4 VS_ShadowMap_AnimMesh(VS_ANIMATION_INPUT_SHADOW input) : SV_POSITION
{
    float fWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    fWeights[0] = input.boneWeights.x;
    fWeights[1] = input.boneWeights.y;
    fWeights[2] = input.boneWeights.z;
    fWeights[3] = 1.0f - fWeights[0] - fWeights[1] - fWeights[2];

    float3 pos = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; ++i)
    {
        pos += fWeights[i] * mul(float4(input.Pos, 1.0f), gmtxAnimationTransforms[input.boneIndices[i]]).xyz;
    }

    return mul(mul(float4(pos, 1.0f), gmtxGameObject), gmtxViewProjection);
}

void PS_ShadowMap_NULL(float4 input : SV_POSITION)
{
	// NULL pixel shader
}


#endif