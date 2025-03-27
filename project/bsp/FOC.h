#ifndef __FOC_H
#define __FOC_H

#include "SVPWM.h"
#include "filter.h"
#include "at32f403a_407.h"              // Device header
#include "mt6701.h"

typedef struct {
	uint16_t ad_A;
	uint16_t ad_B;
    uint16_t voltage_a_offset;
    uint16_t voltage_b_offset;
    uint8_t  Mflag;
} Current_State;

/*
增量式PI调节
*/
struct PI_Struct
{
    /* data */
    float kp;
    float ki;
    float kd;

    float pre;
    float tar;
    float bias;
    float lastBias;
    float out;
    float outMax;
};



typedef struct {
	Current_State current;
	
    float Ualpha, Ubeta; 	
    float Ialpha, Ibeta; 	
    float Ia, Ib, Ic; 			
    float Ua, Ub, Uc; 		
    float Uq, Ud; 			
    float Iq, Id; 			
    float mechanical_angle; 
    float elec_angle;       
    float corr_angle;       
    float zero; 			
	
	struct PI_Struct idPID;
    struct PI_Struct iqPID;
	float tariq;
	float tarid;
	
	float speed_last_angle;
	float speed;
	struct PI_Struct speedPID;
	float tar_speed;
	
	float (*Get_mechanical_angle)(MT6701_t *encoder);
	void (*SetPWM)(float pwm_a, float pwm_b, float pwm_c); 
} FOC_State;

typedef FOC_State *PFOC_State;

extern uint16_t Motor1_AD_Value[2];
extern uint16_t Motor2_AD_Value[2];

extern PFOC_State PMotor_1;
extern PFOC_State PMotor_2;
							 
void FocContorl(PFOC_State pFOC, PSVpwm_State PSVpwm, PLPF_Current  LPF_M);
void angle_init(PFOC_State pFOC);
void M1_adc_tigger(int time_pwm);
void M2_adc_tigger(int time_pwm);
void first_get(PFOC_State pFOC);

void SetCurrentPIDTar(PFOC_State pFOC,float tarid,float tariq);
void SetCurrentPIDParams(PFOC_State pFOC,float kp,float ki,float kd,float outMax);

#endif
