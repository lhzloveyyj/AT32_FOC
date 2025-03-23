#ifndef __SVPWM_H
#define __SVPWM_H

#include "at32f403a_407.h"              // Device header

typedef struct {
	float Ts ;
	uint8_t sector;
	float Ta, Tb, Tc;
	float u1, u2, u3;
	float t1, t2, t3, t4, t5, t6, t7;
	float sum, k_svpwm;
	
	float K;
	
	int times ;
} SVpwm_State;

typedef 	SVpwm_State 	*PSVpwm_State;

extern PSVpwm_State PSVpwm_1 ;
extern PSVpwm_State PSVpwm_2 ;

void SVpwm(PSVpwm_State PSVpwm, float U_alpha, float U_beta);

#endif
