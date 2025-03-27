#include "speed_control.h"
#include "filter.h"

void CalculateSpeed(PFOC_State pFOC, float dt, PLPF_Speed pSpeedFilter)
{
    float angle_diff = (pFOC->mechanical_angle - pFOC->speed_last_angle);

    // 处理跨 0 和 2π 的跳变
    if (angle_diff > 3.14159f)
        angle_diff -= 6.28318f;
    else if (angle_diff < -3.14159f)
        angle_diff += 6.28318f;

    // 计算原始速度
    pFOC->speed = angle_diff / dt;

    // 使用滤波器平滑速度
    LPF_Speed_Update(pSpeedFilter, pFOC->speed, &(pFOC->speed));

    // 更新上一次的角度值
    pFOC->speed_last_angle = pFOC->mechanical_angle;
}

