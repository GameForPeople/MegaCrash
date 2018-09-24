#include "Common.hlsl"

cbuffer cbUIPlayerInfo : register(b4)
{
    float hp;
    float sp;
    float cp;
};

Texture2D gtxtPlayerUI : register(t2);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxViewProjection);
    output.uv = input.uv;
    
    return (output);
}
float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{

    float x_delta = 0;
    float y_delta = 0;

    float4 final_color = float4(0, 0, 0, 0);
    
    float new_background_x_uv = 0.0f;
    float new_background_y_uv = 0.0f;

    float background_y_value = input.uv.y % (1.f / 3.f);
   
    y_delta = InverseLerp(0.0, 0.3, background_y_value);
    new_background_y_uv = lerp(0.0, 0.2, y_delta);

    float2 background_uv = float2(input.uv.x, new_background_y_uv);

    final_color += gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv);

    // 0 ~ 1 사이 정규화 된 값이 들어오면 알아서 길이를 계산하자.
    float hp_delta = InverseLerp(0.0f, 1.f, hp);
    float sp_delta = InverseLerp(0.0f, 1.f, sp);
    float cp_delta = InverseLerp(0.0f, 1.f, cp);

    float min_stat_bar_uv_x = 0.05;
    float max_stat_bar_uv_x = 0.82;
    

     //////////<draw Stat Bar>///////////////////////////////////////////////////////
    // 스탯 바 위아래 약간 잘라내는 값
    float bar_slice_value = 0.05;

    if (min_stat_bar_uv_x < input.uv.x && input.uv.x < max_stat_bar_uv_x)
    {

        y_delta = InverseLerp(0.0, 0.3, background_y_value);
        new_background_y_uv = lerp(0.0, 0.2, y_delta) + 0.2;

        //////////<draw HP>///////////////////////////////////////////////////////
        if (0 + bar_slice_value < input.uv.y && input.uv.y < 0.33 - bar_slice_value)
        {
            // 현재 바가 어느 위치까지 그려저야하는지 계산
            float current_x_value = lerp(min_stat_bar_uv_x, max_stat_bar_uv_x, hp_delta);

            if (input.uv.x < current_x_value)
            {
                
                background_uv = float2(input.uv.x, new_background_y_uv);
        
                // 체력이 50% 이하일 경우 붉은색으로
                final_color += (hp_delta < 0.5) ?
            gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv)
            * gtxtPlayerUI.Sample(gLinearWrapSampler, float2(background_uv.x, background_uv.y + 0.2))
            :
            gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(0, 1, 0, 1);

            }
        }
        /////////<Draw SP>//////////////////////////////////////////////////////
        else if (0.33 + bar_slice_value < input.uv.y && input.uv.y < 0.66 - bar_slice_value)
        {
            float current_x_value = lerp(min_stat_bar_uv_x, max_stat_bar_uv_x, sp_delta);

            if (input.uv.x < current_x_value)
            {
                background_uv = float2(input.uv.x, new_background_y_uv);

                final_color += gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(0, 0.7, 1, 1);
            }
        }
        /////////<Draw CP>//////////////////////////////////////////////////////
        else if (0.66 + bar_slice_value < input.uv.y && input.uv.y < 1.0 - bar_slice_value)
        {

            // 현재 바가 어느 위치까지 그려저야하는지 계산
            float current_x_value = lerp(min_stat_bar_uv_x, max_stat_bar_uv_x, cp_delta);

            if (input.uv.x < current_x_value)
            {
                background_uv = float2(input.uv.x, new_background_y_uv);

                final_color += gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(1, 1, 0, 1);
            }
        }
    }
    //////////<Draw Emblem>////////////////////////////////////////////////////////////////
    if (0.82 < input.uv.x)
    {

        new_background_y_uv = lerp(0.0, 0.2, y_delta) + 0.6;
        x_delta = InverseLerp(0.82, 1.0, input.uv.x);

        if (0 + bar_slice_value < input.uv.y && input.uv.y < 0.33 - bar_slice_value)
        {
            new_background_x_uv = lerp(0.0, 0.2, x_delta);
            background_uv = float2(new_background_x_uv, new_background_y_uv);

            // 체력이 50% 이하일 경우 붉은색으로
            final_color += (hp_delta < 0.5) ?
             gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(1, 0, 0, 1) :
                gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(0, 1, 0, 1);


            final_color += (hp_delta < 0.5) ? gtxtPlayerUI.Sample(gLinearWrapSampler, float2(background_uv.x, background_uv.y + 0.2)) * float4(1, 0, 0, (0.5 + sin(gfTimer * 10.0f) * 0.5)) : 0;

        }
        else if (0.33 + bar_slice_value < input.uv.y && input.uv.y < 0.66 - bar_slice_value)
        {
            new_background_x_uv = lerp(0.0, 0.2, x_delta) + 0.2;
            background_uv = float2(new_background_x_uv, new_background_y_uv);
            final_color += gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(0, 0.7, 1, 1);

            final_color += (sp_delta < 0.5) ? gtxtPlayerUI.Sample(gLinearWrapSampler, float2(background_uv.x, background_uv.y + 0.2)) * float4(1, 0, 0, (0.5 + sin(gfTimer * 10.0f) * 0.5)) : 0;
        }
        else if (0.66 + bar_slice_value < input.uv.y && input.uv.y < 1.0 - bar_slice_value)
        {
            new_background_x_uv = lerp(0.0, 0.2, x_delta) + 0.4;
            background_uv = float2(new_background_x_uv, new_background_y_uv);
            final_color += gtxtPlayerUI.Sample(gLinearWrapSampler, background_uv) * float4(1, 1, 0, 1);

            final_color += (cp_delta < 0.5) ? gtxtPlayerUI.Sample(gLinearWrapSampler, float2(background_uv.x, background_uv.y + 0.2)) * float4(1, 0, 0, (0.5 + sin(gfTimer * 10.0f) * 0.5)) : 0;
        }
    }

    return final_color;
}