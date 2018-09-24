#include "Common.hlsl"

Texture2D gtxtLobbyRoomSlotTexture[10] : register(t2);

cbuffer cbButtonInfo : register(b4)
{
    float2 Center;
    float2 IconSize;
    uint bCursorOn;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;
    float offsetX = IconSize.x * 0.5f;
    float offsetY = IconSize.y * 0.5f;

    if (nVertexID == 0)
    {
        output.position = float4(Center.x - offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(Center.x + offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(Center.x + offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(Center.x - offsetX, Center.y + offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(Center.x + offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(Center.x - offsetX, Center.y - offsetY, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    return output;
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    return gtxtLobbyRoomSlotTexture[0].Sample(gLinearWrapSampler, input.uv);
}