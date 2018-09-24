#include "Common.hlsl"

float movingLine(float2 uv, float2 center, float radius, float speedfactor)
{
    //레이더 선의 각도, 시간에 따라서 각도가 변함
    // 이 각도가 바로 선이 그어질 각도가 된다.
    float theta0 = 90.0 * gfTimer * speedfactor;

    // 중점에서 uv로 향하는 벡터
    float2 d = uv - center;
    // 벡터 길이를 구한다.
    float r = sqrt(dot(d, d));

    // 벡터크기가 지정한 반지름보다 크면 다 잘라버릴 것.
    if (r < radius)
    {
        // 세타 방향으로 향하는, 지정한 반지름 지점을 향하는 벡터를 구한다.
        // 이 벡터영역이 지금 현재 레이더 선의 위치가 된다.
        float2 p = radius * float2(cos(theta0 * PI / 180.0), -sin(theta0 * PI / 180.0));

        // 점p 에서 현재 위치로 향하는 벡터를 구하는데,
        // p의 길이는 0.0과 1.0 의 범위 내에서 (반지름 길이 제한하기) 
        // 직선 위치에서
        // d,p를 내적한 값 / p의 크기만큼 더 나아간 위치의 벡터를 구한다.
        // 이것이 바로 레이더 선이 그려질 위치라고 보면 된다.
        float l = length(d - p * clamp(dot(d, p) / dot(p, p), 0.0, 1.0));

        // 중점에서 현재 uv 좌표로 향하는 벡터의 크기를 1로 정규화한다.
        d = normalize(d);
        //theta0 와의 각도차를 이용해 그라디언트 색상을 계산한다
        // 벡터 d의 아크탄젠트로 라디안 값을 구한후
        // 이를 degree로 변환후 세타제로와 더한 뒤
        // 360으로 나눈다. 이때 반드시 fmod 를 해야한다. modf가 아니라.
        float theta = fmod(180.0 * atan2(d.y, d.x) / PI + theta0, 360.0);

        // 세타 / 그라디언트를 적용할 넓이 , 만약 180으로 나눈다면
        // 180도 영역에 그라디언트를 칠하게 된다. 
        // 최소 그라디언트 색상, 최대 그라디언트 색상으로 클램프시킨다.
        float gradient = clamp(1.0 - theta / 90.0, 0.0, 0.7);

        // gradient에 높은 값이 곱해질 수록 더 색상이 선명해진ㄷ.
        // 베이스 색상 영역 * 밝기 지수 + 그라디언트 색상 * 밝기 지수 를 반환시킨다.
        return Smooth(l, 0.05) * (0.6 + sin(gfTimer) * 0.4) * 3.0 + 4.0 * gradient;
        
    }
    else
        return 0.0;
}

Texture2D<float4> gtxtRader : register(t0);

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float2x2 rotation : ROTATION;
};

VS_OUTPUT VS_MAIN(uint nVertexID : SV_VertexID)
{
    VS_OUTPUT output;

    float client_width = 1600.0f;
    float client_height = 900.0f;
    float rader_size = 500.0f;
    float VerticalSize = rader_size / client_width;
    float HorizonalSize = rader_size / client_height;
    float2 LeftTop = float2(1050.0f / client_width, ((50.0f + rader_size) / client_height) - 1.0f);
    float TopTightenFactor = 0.2f;

    if (nVertexID == 0)
    {
        output.position = float4(LeftTop.x, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 1)
    {
        output.position = float4(LeftTop.x + VerticalSize, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(1.f, 0.f);
    }
    if (nVertexID == 2)
    {
        output.position = float4(LeftTop.x + VerticalSize, LeftTop.y - HorizonalSize, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 3)
    {
        output.position = float4(LeftTop.x, LeftTop.y, 0.0f, 1.0f);
        output.uv = float2(0.f, 0.f);
    }
    if (nVertexID == 4)
    {
        output.position = float4(LeftTop.x + VerticalSize, LeftTop.y - HorizonalSize, 0.0f, 1.0f);
        output.uv = float2(1.f, 1.f);
    }
    if (nVertexID == 5)
    {
        output.position = float4(LeftTop.x, LeftTop.y - HorizonalSize, 0.0f, 1.0f);
        output.uv = float2(0.f, 1.f);
    }

    float theta = atan2(-PlayerLookVector.x, -PlayerLookVector.z);

    output.rotation =
    float2x2(cos(theta), -sin(theta),
             sin(theta), cos(theta));

    return output;
}

float4 PS_MAIN(VS_OUTPUT input) : SV_Target
{
    // 처음엔 hp 를 기반으로 0 ~ 1 사이 보간 t 값을 찾아낸다.
    float first_hp_delta = InverseLerp(0.0f, 1.f, player_hp);
    // 이 hp 델타값을 다시 원호 보간 그래프로 보정한다.
    //float hp_delta = sqrt(1 - pow(1 - first_hp_delta, 2));
    float hp_delta = first_hp_delta;

    float4 result_color = 0;
    float2 centerpos = float2(0.5f, 0.5f);
    float distancefromcenter = distance(input.uv, centerpos);
    // HP 가 줄어들 수록 더 주기를 빠르게 만들 것이다.
    float time_based_hp_factor = lerp(1, 2, 1 - hp_delta);
    // 시간, 0~1 범위에서 계속 주기적으로 값이 증가.
    float time_factor = frac(gfTimer * time_based_hp_factor);
    float current_line_factor = sqrt(1 - pow(1 - time_factor, 2)) * 0.5;

    
    float green_line_thickness = 0.01;

    float2 new_directions[MAX_NUM_PLAYER_DOT];
    float2 new_disatance[MAX_NUM_PLAYER_DOT];

    for (int i = 0; i < num_of_players; ++i)
    {
        new_directions[i] = normalize(OtherPlayersPosition[i].xz - PlayerPosition.xz);
        new_directions[i].x *= -1;
        new_disatance[i] = InverseLerp(0, 10000, distance(PlayerPosition.xz, OtherPlayersPosition[i].xz));
    }

    // 레이더 텍스쳐를 샘플링하여 레이더의 뒷 배경을 그린다.
    float4 sampled_color = gtxtRader.Sample(gLinearWrapSampler, input.uv);
    result_color = (sampled_color) * float4(1.0, 1.0, 1.0, 0.95 + (sin(gfTimer * 55.f)) * 0.01);

     // 회전하는 레이더 선을 그린다.
    float4 moving_line_color = movingLine(input.uv, centerpos, 0.5f, time_based_hp_factor);

    moving_line_color *= lerp(float4(1, 0, 0, 0.25) * 0.25, float4(mechtheme_darker_color, 0.25) * 0.75, hp_delta);

    result_color += (length(moving_line_color.rgb) < 0.01) ? 1 : moving_line_color;
    result_color = clamp(result_color, 0, 1);
   

    result_color *= (current_line_factor - green_line_thickness <=
    distancefromcenter && distancefromcenter <= current_line_factor + green_line_thickness) ?
    float4(0.5, 1, 0.5, 1) : float4(1, 1, 1, 1);
    
    float4 sphereFact = sphere(input.uv.x, input.uv.y, centerpos.x, centerpos.y, 0.002 + sin(gfTimer * time_based_hp_factor) * 0.001);
    result_color += sphereFact;
    result_color = clamp(result_color, 0, 1);

    float theta0 = 90.0 * gfTimer * time_based_hp_factor;
    
    for (int i = 0; i < num_of_players; ++i)
    {
        float2 newpos = mul((new_directions[i] * new_disatance[i]), input.rotation) + 0.5;

        //////////< 나중에 정리할 것.>

        float angle = theta0 * PI / 180.0;
        float2 d = newpos - centerpos;
        float2 p = 0.5f * float2(cos(angle), -sin(angle));
        float l = length(d - p * clamp(dot(d, p) / dot(p, p), 0.0, 0.5));
        float r = sqrt(dot(d, d));
        d = normalize(d);
        float theta = fmod(180.0 * atan2(d.y, d.x) / PI + theta0, 360.0);
        float gradient = clamp(1.0 - theta / 90.0, -0.3, 1.0);


        ///////////

        sphereFact = float4(sphere(input.uv.x, input.uv.y, newpos.x, newpos.y, 0.0005 + max(0, gradient * 0.005)) * red * 2, 1.0);
        
        result_color += (length(sphereFact) > 0.001) ? sphereFact : 0;
        result_color = clamp(result_color, 0, 1);
    }
   

    // 레이더 원 밖에 있는건 전부 0으로
    result_color = (distance(input.uv, centerpos) < 0.5) ? result_color : 0;

    // 약간 투명하게
    result_color.a = clamp(result_color.a, 0, 1);
    result_color *= float4(1, 1, 1, 0.80);

    return result_color;
}