#include "Common.hlsl"

Texture2D gtxtGameoverTexture[10] : register(t2);


// 0 : BackGround
// 1 : NeonWire
// 2 : Victory
// 3 : Lose
// 4 : CloudAlpha
// 5 : BackGroundAlpha

cbuffer cbGameoverInfo : register(b4)
{
    float4 UI_Color;
    float2 Center;
    float2 IconSize;
    float StartTime;
    float IsVictory;
    float2 padding;
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
    float4 backgroundColor = gtxtGameoverTexture[0].Sample(gLinearWrapSampler, input.uv);
    float4 NeonWireColor = gtxtGameoverTexture[1].Sample(gLinearWrapSampler, input.uv);
    float4 CloudAlphaMap = gtxtGameoverTexture[4].Sample(gLinearWrapSampler, input.uv);
    float4 BackGroundAlphaMap = gtxtGameoverTexture[5].Sample(gLinearWrapSampler, input.uv);


    float4 TextColor = float4(0,0,0,0);
    
    if(IsVictory > 0.5f) // 승리 패배여부
        TextColor = gtxtGameoverTexture[2].Sample(gLinearWrapSampler, input.uv);
    else
        TextColor = gtxtGameoverTexture[3].Sample(gLinearWrapSampler, input.uv);

    float4 resultcolor = float4(0, 0, 0, 0);

    NeonWireColor.rgb *= ((0.5 + sin(gfTimer * 5) * 0.5) * 1.2);

    backgroundColor += NeonWireColor * NeonWireColor.a;
				

    backgroundColor.a *= BackGroundAlphaMap + min(0.5, (gfTimer - StartTime) * 0.295 - 0.5);
    backgroundColor.a = saturate(backgroundColor.a);

    TextColor.rgb *= (1.15 + sin(gfTimer * 2) * 0.5) * 0.5;
    TextColor.a *= CloudAlphaMap + min(0.5, (gfTimer - StartTime) * 0.255 - 0.5 );
    TextColor.a = saturate(TextColor.a);

    resultcolor += TextColor * TextColor.a;
    resultcolor += backgroundColor * backgroundColor.a;

    return saturate(resultcolor) ;
}