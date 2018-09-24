#include "Common.hlsl"

cbuffer cbInfo : register(b4)
{
    float2 Center;
    float2 IconSize;
    uint bCursorOn;
};
Texture2D gtxtTitleTexture[10] : register(t2);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;

    float offsetX = IconSize.x * 0.5f;
    float offsetY = IconSize.y * 0.5f;

    if (nVertexID == 0)
    {
        output.pos = float4(Center.x - offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.pos = float4(Center.x + offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.pos = float4(Center.x + offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.pos = float4(Center.x - offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.pos = float4(Center.x + offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.pos = float4(Center.x - offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return output;
}
float4 PS_MAIN(VS_OUTPUT input) : SV_Target
{
    float4 result_color = gtxtTitleTexture[0].Sample(gLinearWrapSampler, input.uv);
    
    
    // 프로퍼티
    const int octaves = 5;
    float lacunarity = 2.0;
    float gain = 0.5;
    
    // 초기값
    float amplitude = 2.028;
    float frequency = 0.5;
    float y = 0;
    int i = 0;
    //
    // 옥타브 루프
    for (i = 0; i < octaves; i++)
    {
        y += amplitude * noise1D(frequency * input.pos.x + gfTimer*5);
        frequency *= lacunarity;
        amplitude *= gain;
    }
    
   
    
    float factor = max(0, result_color.r - result_color.g - result_color.b);
    result_color *= 1 + (factor * 70 / 255.f) * 8.125 * (1 + y);
    

    // 초기값
    amplitude = 2.028;
    frequency = 0.5;
    y = 0;
    //
    // 옥타브 루프
    for (i = 0; i < octaves; i++)
    {
        y += amplitude * noise1D(frequency * input.pos.x + gfTimer*5);
        frequency *= lacunarity;
        amplitude *= gain;
    }


    factor = max(0, result_color.g - result_color.r );
    result_color.r *= 1 + (factor * 40 * (0.5 + sin(gfTimer*2) * 0.5)*2 / 255.f) * 16.125 * (1 + y)*0.1;
    result_color.g *= 1 + (factor * 40 * (0.5 + sin(gfTimer * 2) * 0.5) * 2 / 255.f) * 16.125 * (1 + y)*0.6;
    result_color.b *= 1 + (factor * 40 * (0.5 + sin(gfTimer * 2) * 0.5) * 2 / 255.f) * 16.125 * (1 + y)*0.3;



    return result_color;
}