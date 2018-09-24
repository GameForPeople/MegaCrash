#include "Common.hlsl"

struct VS_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output;
    output.PosL = input.position;
    output.PosH = mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxViewProjection);

    return (output);
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    return gtxtSkyBox.Sample(gLinearClampSampler, input.PosL);
}

PS_DEFFERED_OUTPUT PS_DEFFERED(VS_OUTPUT input)
{
    PS_DEFFERED_OUTPUT output;
    output.posW_bActive.w = 0.0f;
    output.diffuse = gtxtSkyBox.Sample(gLinearClampSampler, input.PosL);
    return output;
}