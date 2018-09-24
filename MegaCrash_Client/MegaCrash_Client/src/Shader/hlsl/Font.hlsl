#include "Common.hlsl"

struct VS_INPUT
{
    float4 pos : POSITION;
    float4 uv : TEXCOORD;
    float4 color : COLOR;
};

struct GS_INPUT
{
    float4 pos : POSITION;
    float4 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    uint primID : SV_PrimitiveID;
};

Texture2D gtxtFontTexture : register(t2);

GS_INPUT VS_MAIN(VS_INPUT input, uint nVertexID : SV_VertexID)
{
    GS_INPUT output;
    output.pos = input.pos;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[maxvertexcount(4)]
void GS_MAIN(
	  point GS_INPUT input[1]
	, uint primID : SV_PrimitiveID
	, inout TriangleStream<PS_INPUT> outStream)
{
    float4 positions[4] = {
          float4(input[0].pos.x, input[0].pos.y - input[0].pos.w, 0, 1)
        , float4(input[0].pos.x, input[0].pos.y, 0, 1)
        , float4(input[0].pos.x + input[0].pos.z, input[0].pos.y - input[0].pos.w, 0, 1)
        , float4(input[0].pos.x + input[0].pos.z, input[0].pos.y, 0, 1)
    };

    float2 newUV[4] =
    {
        float2(input[0].uv.x, input[0].uv.y + input[0].uv.w)
        , float2(input[0].uv.x, input[0].uv.y)
        , float2(input[0].uv.x + input[0].uv.z, input[0].uv.y + input[0].uv.w)
        , float2(input[0].uv.x + input[0].uv.z, input[0].uv.y )
    };

    PS_INPUT output;
    for (int i = 0; i < 4; i++)
    {
        output.pos = positions[i];
        output.uv = newUV[i];
        output.color = input[0].color;
        output.primID = primID;
        outStream.Append(output);
    }
}


float4 PS_MAIN(PS_INPUT input) : SV_Target
{
    return float4(input.color.rgb, input.color.a * gtxtFontTexture.Sample(gAnisotropicWrapSampler, input.uv).a);
}
