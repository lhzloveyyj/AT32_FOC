#ifndef __FOC_H
#define __FOC_H

#include "math.h"
#include "at32f403a_407.h"              // Device header

typedef struct {
    float Ualpha, Ubeta; 	
    float Ialpha, Ibeta; 	
    float Ia, Ib; 			
    float Ua, Ub, Uc; 		
    float Uq, Ud; 			
    float Iq, Id; 			
    float mechanical_angle; 
    float elec_angle;       
    float corr_angle;       
    float zero; 			
	
	float (*Get_mechanical_angle)(void);
	void (*SetPWM)(float pwm_a, float pwm_b, float pwm_c); 
} FOC_State;

typedef FOC_State *PFOC_State;

extern uint16_t AD_Value[2];
extern uint16_t voltage_a_offset, voltage_b_offset;

extern PFOC_State PMotor_1;

extern PFOC_State PMotor_2;
							 
void setPhaseVoltage(uint16_t V_a, uint16_t V_b, PFOC_State pFOC );		
void angle_init(PFOC_State pFOC);
void adc_tigger(int time_pwm);
void first_get(uint16_t *Ua_offset,uint16_t *Ub_offset);

#endif
