#ifndef __FOC_H
#define __FOC_H

#include "math.h"
#include "at32f403a_407.h"              // Device header

extern float Ua,Ub,Uc;
extern float Ia , Ib ;
extern int voltage_a ,voltage_b ;
extern float Ialpha, Ibeta;
extern float Iq , Id;

extern uint16_t AD_Value[2];
extern uint16_t voltage_a_offset, voltage_b_offset;

float Angle_limit(float angle);								 	// 把角度值限制在 [0, 2pi]
void setPhaseVoltage(uint16_t V_a, uint16_t V_b, float angle);		// 驱动函数
void angle_init(float (*read_angle_func)(void));
void adc_tigger(int time_pwm);
void first_get(uint16_t *Ua_offset,uint16_t *Ub_offset);

#endif
