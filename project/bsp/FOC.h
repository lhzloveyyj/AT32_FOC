#ifndef __FOC_H
#define __FOC_H

#include "math.h"
#include "at32f403a_407.h"              // Device header

extern float Ua,Ub,Uc;
extern uint16_t AD_Value[2];

float Angle_limit(float angle);								 	// 把角度值限制在 [0, 2pi]
void setPhaseVoltage(uint16_t Ia, uint16_t Ib, float angle);		// 驱动函数
void angle_init(float (*read_angle_func)(void));
void adc_tigger(int time_pwm);

#endif
