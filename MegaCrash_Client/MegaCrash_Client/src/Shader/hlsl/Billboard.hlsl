#include "Common.hlsl"

Texture2D gtxtParticle : register(t2);

struct VS_INPUT
{
    float3 posW : POSITION;
    float2 sizeW : SIZE;
    float2 currImg : CURRENT_IMAGE;
    float2 dividedUV : DIVIDED_TEXCOORD;
};

struct GS_INPUT
{
    float3 centerW : POSITION;
    float2 sizeW : SIZE;
    float2 currImg : CURRENT_IMAGE;
    float2 dividedUV : DIVIDED_TEXCOORD;
};

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD;
    uint primID : SV_PrimitiveID;
};

GS_INPUT VS_MAIN(VS_INPUT input)
{
    GS_INPUT output;
    output.centerW = input.posW;
    output.sizeW = input.sizeW;
    output.currImg = input.currImg;
    output.dividedUV = input.dividedUV;
    return output;
}

[maxvertexcount(4)]
void GS_MAIN(
	  point GS_INPUT input[1]
	, uint primID : SV_PrimitiveID
	, inout TriangleStream<PS_INPUT> outStream)
{
    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float3 vLook = gvCameraPosition.xyz - input[0].centerW;
    vLook = normalize(vLook);
    float3 vRight = cross(vUp, vLook);

    float fHalfW = input[0].sizeW.x * 0.5f;
    float fHalfH = input[0].sizeW.y * 0.5f;
    float3 vWidth = fHalfW * vRight;
    float3 vHeight = fHalfH * vUp;

    float4 pVertices[4] = {
          float4(input[0].centerW + vWidth - vHeight, 1.0f)
        , float4(input[0].centerW + vWidth + vHeight, 1.0f)
        , float4(input[0].centerW - vWidth - vHeight, 1.0f)
        , float4(input[0].centerW - vWidth + vHeight, 1.0f)
    };

    float base_u = input[0].dividedUV.x;
    float base_v = input[0].dividedUV.y;
    float curr_u = uint(input[0].currImg.x) * base_u;
    float curr_v = uint(input[0].currImg.y) * base_v;

    float2 pUVs[4] = {
          float2(curr_u, curr_v + base_v)
		, float2(curr_u, curr_v)
		, float2(curr_u + base_u, curr_v + base_v)
		, float2(curr_u + base_u, curr_v)
    };

    PS_INPUT output;
    for (int i = 0; i < 4; i++)
    {
        output.posW = pVertices[i].xyz;
        output.posH = mul(pVertices[i], gmtxViewProjection);
        output.normalW = vLook;
        output.uv = pUVs[i];
        output.primID = primID;
        outStream.Append(output);
    }
}

float4 PS_MAIN(PS_INPUT input) : SV_Target
{
    return gtxtParticle.Sample(gLinearWrapSampler, input.uv);
}