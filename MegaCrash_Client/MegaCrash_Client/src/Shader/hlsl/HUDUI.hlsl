#include "Common.hlsl"

cbuffer cbUIPlayerInfo : register(b4)
{
    float hp;
    float sp;
    float cp;
};

Texture2D<float4> gtxtUI : register(t0);

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;

    if (nVertexID == 0)
    {
        output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return output;
}
float4 PS_MAIN(VS_OUTPUT output) : SV_Target
{
    float hp_delta = InverseLerp(0.0f, 1.f, player_hp);
    float4 result_color = 0;

    float4 sampled_color = gtxtUI.Sample(gLinearWrapSampler, output.uv);

    float4 red_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    float4 blended_color_on_HP = sampled_color * lerp(red_color, float4(mechtheme_color, 1), hp_delta);
    
    result_color = blended_color_on_HP * (1, 1, 1, 0.85 + (sin(gfTimer * 5.f)) * 0.2);

    return result_color;
}