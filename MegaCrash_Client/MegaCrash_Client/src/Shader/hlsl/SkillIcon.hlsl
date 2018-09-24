#include "Common.hlsl"


Texture2D gtxtSkillColorTexture[10] : register(t2);
Texture2D gtxtSkillGreyTexture[10] : register(t12);

cbuffer cbSkillIconInfo : register(b4)
{
    float2 LeftTop;
    float2 IconSize;
    float  cool_time;
    float3 padding;
};


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
        output.position = float4(LeftTop.x, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(LeftTop.x + IconSize.x, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(LeftTop.x + IconSize.x, LeftTop.y - IconSize.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(LeftTop.x, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(LeftTop.x + IconSize.x, LeftTop.y - IconSize.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(LeftTop.x, LeftTop.y - IconSize.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return output;
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
  
  
    float3 color = (gtxtSkillColorTexture[0].Sample(gLinearWrapSampler, input.uv).xyz);
    float3 grey = (gtxtSkillGreyTexture[0].Sample(gLinearWrapSampler, input.uv).xyz);
    float3 result_color = grey;
    float2 new_uv = input.uv - 0.5;

    float new_cool_time = cool_time;


    float s = sin(radians(90));
    float c = cos(radians(90));
    float2x2 rotationMatrix = float2x2(c, -s, s, c);

    new_uv = mul(new_uv, rotationMatrix);
    new_uv += 0.5;
    float sweep = (atan2(new_uv.y * 2 - 1, new_uv.x * 2 - 1) / PI) / 2 + 0.5;

    if (new_cool_time > sweep)
        result_color = color;
    
    float dist = abs(new_cool_time - sweep);
    if (dist < 0.005 + random(gfTimer + length(input.uv) ) * 0.005)
    {
        result_color 
        *=1 + (1 - InverseLerp(0, 0.005 + random(gfTimer + length(input.uv)) * 0.005, dist))*10;
    }

    // 스킬 쿨타임 찬 후 번쩍 시간
    float flicker_time = 0.1;
    float alpha = 0.6;


    if (1 < new_cool_time)
    {
        alpha += 0.15 + (0.5 + sin(gfTimer * 5) * 0.5) * 0.2;

        if (new_cool_time < 1 + flicker_time)
        {
            float delta = InverseLerp(1, 1 + flicker_time, new_cool_time);
            result_color += 1 - delta;
        }
        else
        {
            result_color *= 1 + (0.5 + sin(gfTimer*5) * 0.5);
        }
    }
    return float4(result_color, alpha);
}