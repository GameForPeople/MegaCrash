#ifndef __COMMON_HLSL__
#define __COMMON_HLSL__

#define MAX_NUM_PLAYER_DOT     8
cbuffer cbShaderHelper : register(b6)
{
    float gfTimer;
    uint num_of_players;
    float player_hp;
    uint padding1;
    float4 PlayerLookVector;
    float4 PlayerPosition;
    float4 OtherPlayersPosition[MAX_NUM_PLAYER_DOT];
};
Texture2D gtxtShaderHelper : register(t2);

cbuffer cbCameraInfo : register(b2)
{
    matrix gmtxViewProjection : packoffset(c0);
    float3 gvCameraPosition : packoffset(c4);
};

cbuffer cbGameObjectInfo : register(b3)
{
    matrix gmtxGameObject : packoffset(c0);
    uint gnMaterial : packoffset(c4);
};

#define NUM_JOINTS 128
cbuffer cbAnimationInfo : register(b4)
{
    matrix gmtxAnimationTransforms[NUM_JOINTS] : packoffset(c0);
};

TextureCube gtxtSkyBox : register(t0);

SamplerState gLinearWrapSampler : register(s0);
SamplerState gLinearClampSampler : register(s1);
SamplerState gPointWrapSampler : register(s2);
SamplerState gPointClampSampler : register(s3);
SamplerState gAnisotropicWrapSampler : register(s4);
SamplerState gAnisotropicClampSampler : register(s5);
SamplerComparisonState gShadowSampler : register(s6);

#include "Light.hlsl"
#include "ShaderMathHelper.hlsl"

struct PS_DEFFERED_OUTPUT
{
    float4 diffuse              : SV_TARGET0;
    float4 posW_bActive         : SV_TARGET1;
    float4 ShadowposW           : SV_TARGET2;
    float4 normW_PostRimType    : SV_TARGET3;
    float4 SpecC_GlossF         : SV_TARGET4;
    float4 emissive             : SV_TARGET5;
};
#define POST_RIM_TYPE_NONE 0
#define POST_RIM_TYPE_CUBE 1
#define POST_RIM_TYPE_STAGE 2
#endif