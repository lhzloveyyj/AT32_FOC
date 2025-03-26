#include "speed_control.h"

void CalculateSpeed(PFOC_State pFOC, float dt)
{

    float angle_diff = (pFOC->mechanical_angle - pFOC->speed_last_angle);

    if (angle_diff > 3.14159f)
        angle_diff -= 6.28318f;
    else if (angle_diff < -3.14159f)
        angle_diff += 6.28318f;

        pFOC->speed = angle_diff / dt;

    pFOC->speed_last_angle = pFOC->mechanical_angle;
}
