#include "speed_control.h"
#include "filter.h"

void CalculateSpeed(PFOC_State pFOC, float dt, PLPF_Speed pSpeedFilter)
{
    float angle_diff = (pFOC->mechanical_angle - pFOC->speed_last_angle);

    // ����� 0 �� 2�� ������
    if (angle_diff > 3.14159f)
        angle_diff -= 6.28318f;
    else if (angle_diff < -3.14159f)
        angle_diff += 6.28318f;

    // ����ԭʼ�ٶ�
    pFOC->speed = angle_diff / dt;

    // ʹ���˲���ƽ���ٶ�
    LPF_Speed_Update(pSpeedFilter, pFOC->speed, &(pFOC->speed));

    // ������һ�εĽǶ�ֵ
    pFOC->speed_last_angle = pFOC->mechanical_angle;
}

