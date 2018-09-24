#include "Common.hlsl"

cbuffer cbSkillIconInfo : register(b4)
{
    float2 Center;
    float2 BaseRadius;
    float DamagedTime;
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

    
    // 카메라 흔드는 도중 번쩍 시간
    float flicker_time = 0.1;
    float2 new_radius = BaseRadius;

    
    if (DamagedTime + flicker_time > gfTimer)
    {
        float delta = InverseLerp(DamagedTime, DamagedTime + flicker_time, gfTimer);
        float factor = smoothstep(1, 0, delta);
        new_radius.x += factor * (50.0f / 1600.0f);
        new_radius.y += factor * (50.0f / 900.0f);
    }

    if (nVertexID == 0)
    {
        output.position = float4(Center.x - new_radius.x, Center.y + new_radius.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(Center.x + new_radius.x, Center.y + new_radius.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(Center.x + new_radius.x, Center.y - new_radius.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(Center.x - new_radius.x, Center.y + new_radius.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(Center.x + new_radius.x, Center.y - new_radius.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(Center.x - new_radius.x, Center.y - new_radius.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }





    return output;
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float3 result_color = float3(0, 0, 0);

    float3 center_cross = float3(0, 0, 0);
				// 메인 십자가
    float main_cross_size = 0.6;
    center_cross += Drawcross(input.uv, main_cross_size);
    center_cross = min(center_cross, 1);

				// 십자가 안쪽 모서리에 홈을 판다.
    float first_slot_offset = main_cross_size * 0.125;
    float first_slot_size = main_cross_size * 0.075;
    center_cross -= Drawbox(input.uv + float2(first_slot_offset, first_slot_offset), float2(first_slot_size, first_slot_size));
    center_cross -= Drawbox(input.uv + float2(-first_slot_offset, first_slot_offset), float2(first_slot_size, first_slot_size));
    center_cross -= Drawbox(input.uv + float2(first_slot_offset, -first_slot_offset), float2(first_slot_size, first_slot_size));
    center_cross -= Drawbox(input.uv + float2(-first_slot_offset, -first_slot_offset), float2(first_slot_size, first_slot_size));


    float second_slot_offset = main_cross_size * 0.075;
    float second_slot_size = main_cross_size * 0.075;
    center_cross -= Drawbox(input.uv + float2(second_slot_offset, second_slot_offset), float2(second_slot_size, second_slot_size));
    center_cross -= Drawbox(input.uv + float2(-second_slot_offset, second_slot_offset), float2(second_slot_size, second_slot_size));
    center_cross -= Drawbox(input.uv + float2(second_slot_offset, -second_slot_offset), float2(second_slot_size, second_slot_size));
    center_cross -= Drawbox(input.uv + float2(-second_slot_offset, -second_slot_offset), float2(second_slot_size, second_slot_size));

    float third_slot_offset = main_cross_size * 0.0275;
    float third_slot_size = main_cross_size * 0.0875;
    center_cross -= Drawbox(input.uv + float2(third_slot_offset, third_slot_offset), float2(third_slot_size, third_slot_size));
    center_cross -= Drawbox(input.uv + float2(-third_slot_offset, third_slot_offset), float2(third_slot_size, third_slot_size));
    center_cross -= Drawbox(input.uv + float2(third_slot_offset, -third_slot_offset), float2(third_slot_size, third_slot_size));
    center_cross -= Drawbox(input.uv + float2(-third_slot_offset, -third_slot_offset), float2(third_slot_size, third_slot_size));

				// 안쪽 크로스헤어 팔 부분을 파서 테두리효과
    float main_cross_half_size = main_cross_size * 0.5;
    float cross_arm_slot_size = main_cross_size * 0.0875;
    center_cross -= Drawbox(input.uv + float2(main_cross_half_size, 0), float2(cross_arm_slot_size, cross_arm_slot_size));
    center_cross -= Drawbox(input.uv + float2(-main_cross_half_size, 0), float2(cross_arm_slot_size, cross_arm_slot_size));
    center_cross -= Drawbox(input.uv + float2(0, main_cross_half_size), float2(cross_arm_slot_size, cross_arm_slot_size));
    center_cross -= Drawbox(input.uv + float2(0, -main_cross_half_size), float2(cross_arm_slot_size, cross_arm_slot_size));

    float inner_offset = main_cross_size * 0.275;
    float inner_thickness = main_cross_size - main_cross_size * 0.1;

    center_cross -= Drawbox(input.uv + float2(-inner_offset, 0), float2(inner_thickness * 0.45, inner_thickness * 0.25));
    center_cross -= Drawbox(input.uv + float2(inner_offset, 0), float2(inner_thickness * 0.45, inner_thickness * 0.25));
    center_cross -= Drawbox(input.uv + float2(0, inner_offset), float2(inner_thickness * 0.25, inner_thickness * 0.45));
    center_cross -= Drawbox(input.uv + float2(0, -inner_offset), float2(inner_thickness * 0.25, inner_thickness * 0.45));
    center_cross = max(0, center_cross);

    float inn_inner_additional_cross_arm_thickness = main_cross_half_size * 0.01;
    center_cross += Drawbox(input.uv + float2(main_cross_half_size + sin(gfTimer * 5) * 0.02, 0), float2(main_cross_half_size, inn_inner_additional_cross_arm_thickness));
    center_cross += Drawbox(input.uv + float2(-main_cross_half_size - sin(gfTimer * 5) * 0.02, 0), float2(main_cross_half_size, inn_inner_additional_cross_arm_thickness));
    center_cross += Drawbox(input.uv + float2(0, main_cross_half_size + sin(gfTimer * 5) * 0.02), float2(inn_inner_additional_cross_arm_thickness, main_cross_half_size));
    center_cross += Drawbox(input.uv + float2(0, -main_cross_half_size - sin(gfTimer * 5) * 0.02), float2(inn_inner_additional_cross_arm_thickness, main_cross_half_size));



    float3 main_line_square = float3(0, 0, 0);

    float s = sin(radians(45));
    float c = cos(radians(45));
    float2x2 rotationMatrix = float2x2(c, -s, s, c);

    main_line_square += Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size, main_cross_size));
    main_line_square -= Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size, main_cross_size) * 0.995);
    main_line_square -= Drawcross(input.uv, main_cross_size * 1.2);

    main_line_square += Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size, main_cross_size) * 0.985);
    main_line_square -= Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size, main_cross_size) * 0.975);
    main_line_square -= Drawcross(input.uv, main_cross_size * 1.195);

    main_line_square -= Drawbox(input.uv, float2(main_cross_size * 1.5, main_cross_size * 0.1) * 0.975);
    main_line_square -= Drawbox(input.uv, float2(main_cross_size * 0.1, main_cross_size * 1.5) * 0.975);
    main_line_square = max(0, main_line_square);

    result_color.xyz += main_line_square;

    result_color *= mechtheme_color;


    s = sin(radians(gfTimer * 100));
    c = cos(radians(gfTimer * 100));
    rotationMatrix = float2x2(c, -s, s, c);
    center_cross += Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size * 0.25, main_cross_size * 0.25)) * red.xyz;
    center_cross -= Drawbox(mul(input.uv - 0.5, rotationMatrix) + 0.5, float2(main_cross_size * 0.15, main_cross_size * 0.15)) * red.xyz;
    result_color.xyz += center_cross;


    


    //------------------

   // 카메라 흔드는 도중 번쩍 시간
   float flicker_time = 0.1;

    
    
   if (DamagedTime + flicker_time > gfTimer)
   {
        float delta = InverseLerp(DamagedTime, DamagedTime + flicker_time, gfTimer);
        result_color *= 1 + (1 - delta)*5;

    }
   
    return float4(result_color, smoothstep(0.0,0.1, length(result_color))*0.85);
}