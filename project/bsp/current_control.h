#ifndef __PID_H
#define __PID_H

#include "at32f403a_407.h"              // Device header
#include "FOC.h"

void CurrentPIControlID(PFOC_State pFOC);
void CurrentPIControlIQ(PFOC_State pFOC);
void SetCurrentPIDTar(PFOC_State pFOC,float tarid,float tariq);
void SetCurrentPIDParams(PFOC_State pFOC,float kp,float ki,float kd,float outMax);

#endif
