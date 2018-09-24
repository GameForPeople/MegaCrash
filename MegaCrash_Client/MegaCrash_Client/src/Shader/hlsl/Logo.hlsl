#include "Common.hlsl"

cbuffer cbInfo : register(b4)
{
};
Texture2D gtxtLogoTexture[10] : register(t2);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;

    if (nVertexID == 0) {
        output.pos = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1) {
        output.pos = float4(+1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2) {
        output.pos = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3) {
        output.pos = float4(-1.0f, +1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4) {
        output.pos = float4(+1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5) {
        output.pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return output;
}
float4 PS_MAIN(VS_OUTPUT input) : SV_Target
{

    float4 result_color = gtxtLogoTexture[0].Sample(gLinearWrapSampler, input.uv);
    return result_color;
}