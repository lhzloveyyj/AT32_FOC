#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

#include "foc.h"

#include "at32f403a_407.h"  // AT32F403A/407 Í·ÎÄ¼þ

void CalculateSpeed(PFOC_State pFOC, float dt, PLPF_Speed pSpeedFilter);
void SpeedPIControl(PFOC_State pFOC);
void SetSpeedPIDTar(PFOC_State pFOC,float tarspeed);
void SetSpeedPIDParams(PFOC_State pFOC,float kp,float ki,float kd,float outMax);


#endif 
