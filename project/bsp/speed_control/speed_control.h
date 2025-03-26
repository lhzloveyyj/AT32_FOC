#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

#include "foc.h"

#include "at32f403a_407.h"  // AT32F403A/407 ͷ�ļ�

void CalculateSpeed(PFOC_State pFOC, float dt);

#endif 
