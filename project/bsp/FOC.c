/******************************************************************************
MOTOE_1
pwm3 - time4_ch3 - Ua - Ia - ADC1_IN9 - AD_Value[1]
pwm2 - time4_ch2 - Ub - Ib - ADC1_IN8 - AD_Value[0]
pwm1 - time4_ch1 - Uc - Ic

MOTOE_2
pwm6 - time2_ch4 - Ua - Ia - ADC3_IN1 - AD_Value[1]
pwm5 - time2_ch2 - Ub - Ib - ADC3_IN0 - AD_Value[0]
pwm4 - time2_ch1 - Uc - Ic

******************************************************************************/

#include "at32f403a_407.h"              // Device header
#include "foc.h"
#include "math.h"
#include "fast_sin.h"
#include "delay.h"
#include "stdio.h"
#include "mt6701.h"
#include "PID.h"
#include "foc_config.h"
#include "SVpwm.h"
                                            
uint16_t Motor1_AD_Value[2] = {0};
uint16_t Motor2_AD_Value[2] = {0};


// PI 控制器初始化
PIController pi_Id = {0.01f, 0.1f, 0.0f, 0.0f, 10.0f, -10.0f};
PIController pi_Iq = {0.01f, 0.1f, 0.0f, 0.0f, 6.0f, -6.0f};

static void setpwm(FOC_State *foc);
static float limit(float in_vo, float low, float high);
static float Angle_limit(float angle);
static void _electricalAngle(PFOC_State pFOC) ;
static void strong_drag_Angle(PFOC_State pFOC);
static void getCorrectedElectricalAngle(PFOC_State pFOC) ;
static void strong_drag(PFOC_State pFOC) ;
void angle_init(PFOC_State pFOC);
void M1_adc_tigger(int time_pwm);
void M2_adc_tigger(int time_pwm);
void first_get(PFOC_State pFOC);
static void clarke_transform(PFOC_State pFOC) ;
static void park_transform(PFOC_State pFOC);
static void inv_park_transform(PFOC_State pFOC);
static void inv_clarke_transform(PFOC_State pFOC);
void FocContorl(PFOC_State pFOC);
static void setpwm1_channel(float pwm_a, float pwm_b, float pwm_c);
static void setpwm2_channel(float pwm_a, float pwm_b, float pwm_c);

// 定义motor_1
FOC_State Motor_1 = {
	.current = {
        .ad_A = 0,
        .ad_B = 0,
        .voltage_a_offset = 0,
        .voltage_b_offset = 0,
        .Mflag = 1
    },
    .Ualpha = 0.0f, .Ubeta = 0.0f, 	
    .Ialpha = 0.0f, .Ibeta = 0.0f, 	
    .Ia = 0.0f, .Ib = 0.0f, 			
    .Ua = 0.0f, .Ub = 0.0f, .Uc = 0.0f, 		
    .Uq = 0.0f, .Ud = 0.0f, 			
    .Iq = 0.0f, .Id = 0.0f, 			
    .mechanical_angle = 0.0f, 
    .elec_angle = 0.0f,       
    .corr_angle = 0.0f,       
    .zero = 0.0f, 			

    .Get_mechanical_angle = MT_1_ReadAngle	,
	.SetPWM = setpwm1_channel	,
};

PFOC_State PMotor_1 = &Motor_1;

// 定义motor_2
FOC_State Motor_2 = {
	.current = {
        .ad_A = 0,
        .ad_B = 0,
        .voltage_a_offset = 0,
        .voltage_b_offset = 0,
        .Mflag = 2
    },
    .Ualpha = 0.0f, .Ubeta = 0.0f, 	
    .Ialpha = 0.0f, .Ibeta = 0.0f, 	
    .Ia = 0.0f, .Ib = 0.0f, 			
    .Ua = 0.0f, .Ub = 0.0f, .Uc = 0.0f, 		
    .Uq = 0.0f, .Ud = 0.0f, 			
    .Iq = 0.0f, .Id = 0.0f, 			
    .mechanical_angle = 0.0f, 
    .elec_angle = 0.0f,       
    .corr_angle = 0.0f,       
    .zero = 0.0f, 			

    .Get_mechanical_angle = MT_2_ReadAngle	,
	.SetPWM = setpwm2_channel
};

PFOC_State PMotor_2 = &Motor_2;

// 限幅函数
static float limit(float in_vo, float low, float high) {
    if (in_vo >= high) return high;
    if (in_vo <= low) return low;
    return in_vo;
}

// 角度限幅函数，保证角度在 [0, 2PI] 之间
static float Angle_limit(float angle) {
    float a = fmod(angle, _2PI);
    return a >= 0.0f ? a : (a + _2PI);
}

// 计算电角度
static void _electricalAngle(PFOC_State pFOC) {
    pFOC->mechanical_angle = pFOC->Get_mechanical_angle();
    pFOC->elec_angle = Angle_limit(pFOC->mechanical_angle * POLE_PAIRS);
}

// 减去零电位并归一化
static void getCorrectedElectricalAngle(PFOC_State pFOC) 
{
    _electricalAngle(pFOC);
    pFOC->corr_angle = Angle_limit(pFOC->elec_angle - pFOC->zero);
    if (fabs(pFOC->corr_angle - _2PI) < EPSILON) pFOC->corr_angle = 0.0f;
}

// 强拖电角度
static void strong_drag_Angle(PFOC_State pFOC)
{
    pFOC->mechanical_angle = 0.0f;
	pFOC->elec_angle = Angle_limit(pFOC->mechanical_angle * POLE_PAIRS);
	pFOC->corr_angle = Angle_limit(pFOC->elec_angle - pFOC->zero);
	if (fabs(pFOC->corr_angle - _2PI) < EPSILON) pFOC->corr_angle = 0.0f;
}


// 强拖模式
static void strong_drag(PFOC_State pFOC) {
    
	pFOC->Ud = 2.0f;
	pFOC->Uq = 0.0f;

	
    strong_drag_Angle(pFOC);

    
    inv_park_transform(pFOC);

    
    inv_clarke_transform(pFOC);
    
    setpwm(pFOC);
}

// 角度初始化
void angle_init(PFOC_State pFOC)
{	
    strong_drag(pFOC); 
    delay_ms(1000);

    float sum = 0;
    int samples = 10;
	_electricalAngle(pFOC);
    for (int i = 0; i < samples; i++) {
		_electricalAngle(pFOC);
        /**************************************************/
        // BUG : 
        /**************************************************/
        printf("_electricalAngle : %lf\r\n", pFOC->elec_angle);
        sum += pFOC->elec_angle;
        delay_ms(10); 
    }
    pFOC->zero = sum / samples;
	
	getCorrectedElectricalAngle(pFOC);
	
    printf("pFOC->zero is : %f, pFOC->corr_angle is : %lf\r\n", pFOC->zero, pFOC->corr_angle);
	if(0 == pFOC->corr_angle){
		printf("motor %d angle_init OK !\r\n",pFOC->current.Mflag);
	}
}

//定时器触发ADC采样
void M1_adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_4, time_pwm-10);
}

void M2_adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, time_pwm-10);
}

//获取电压偏置
void first_get(PFOC_State pFOC)
{
	pFOC->current.ad_B = 0;
	pFOC->current.ad_A = 0;
	
	for(int i=0;i<16;i++)
	{	
		if(1 == pFOC->current.Mflag){
		pFOC->current.ad_B += Motor1_AD_Value[0];
		pFOC->current.ad_A += Motor1_AD_Value[1];
		}
		
		else if(2 == pFOC->current.Mflag){
		pFOC->current.ad_B += Motor2_AD_Value[0];
		pFOC->current.ad_A += Motor2_AD_Value[1];
		}
		
//		printf("%d	Ua voltage offset is %d \r\n Ub voltage offset is  %d\r\n",i,AD_Value[1],AD_Value[0]);
	}
	pFOC->current.voltage_a_offset = pFOC->current.ad_A>>4;
	pFOC->current.voltage_b_offset = pFOC->current.ad_B>>4;
	
	printf("motor %d : Ua voltage offset is %d , Ub \r\n voltage offset is  %d\r\n",
		pFOC->current.Mflag,pFOC->current.voltage_a_offset,pFOC->current.voltage_b_offset);
}

//电机1设置PWM占空比
static void setpwm1_channel(float pwm_a, float pwm_b, float pwm_c)
{
	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, pwm_a * TIME2_4_PWM);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_2, pwm_b * TIME2_4_PWM);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, pwm_c * TIME2_4_PWM);
}

//电机2设置PWM占空比
static void setpwm2_channel(float pwm_a, float pwm_b, float pwm_c)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_a * TIME2_4_PWM);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * TIME2_4_PWM);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_c * TIME2_4_PWM);
}
//设置PWM
static void setpwm(PFOC_State pFOC)
{
    pFOC->Ua = limit(pFOC->Ua, 0.0f, VOLTAGE_HIGH);
    pFOC->Ub = limit(pFOC->Ub, 0.0f, VOLTAGE_HIGH);
    pFOC->Uc = limit(pFOC->Uc, 0.0f, VOLTAGE_HIGH);
    
    float pwm_a = limit(pFOC->Ua / UDC, 0.0f, 1.0f);
    float pwm_b = limit(pFOC->Ub / UDC, 0.0f, 1.0f);
    float pwm_c = limit(pFOC->Uc / UDC, 0.0f, 1.0f);
	
    pFOC->SetPWM(pwm_a,pwm_b,pwm_c);
}

//设置PWM
static void setSVpwm(PFOC_State pFOC, PSVpwm_State PSVpwm)
{
	
    pFOC->SetPWM(PSVpwm->Ta, PSVpwm->Tb, PSVpwm->Tc);
}

// Clarke 变换
static void clarke_transform(PFOC_State pFOC) {
	//Uq给正时Iq为负，因此对电流取反，保证一致
	pFOC->Ia = -pFOC->Ia;
	pFOC->Ib = -pFOC->Ib;
	
    pFOC->Ialpha = pFOC->Ia;
    pFOC->Ibeta = (1 / SQRT3) * (pFOC->Ia + 2 * pFOC->Ib);
}

// Park 变换
static void park_transform(PFOC_State pFOC) {
    pFOC->Id = pFOC->Ialpha * fast_cos(pFOC->corr_angle) + pFOC->Ibeta * fast_sin(pFOC->corr_angle);  
    pFOC->Iq = -pFOC->Ialpha * fast_sin(pFOC->corr_angle) + pFOC->Ibeta * fast_cos(pFOC->corr_angle);
}

// 逆 Park 变换
static void inv_park_transform(PFOC_State pFOC)
{
	pFOC->Ualpha = -pFOC->Uq * fast_sin(pFOC->corr_angle) + pFOC->Ud * fast_cos(pFOC->corr_angle);
	pFOC->Ubeta  =  pFOC->Uq * fast_cos(pFOC->corr_angle) + pFOC->Ud * fast_sin(pFOC->corr_angle);
}

// 逆 Clarke 变换
static void inv_clarke_transform(PFOC_State pFOC)
{
	pFOC->Ua = pFOC->Ualpha + UDC/2;
	pFOC->Ub = (SQRT3 * pFOC->Ubeta - pFOC->Ualpha)/2 + UDC/2;
	pFOC->Uc = (-SQRT3 * pFOC->Ubeta - pFOC->Ualpha)/2 + UDC/2;
}

// FOC 控制主函数
void FocContorl(PFOC_State pFOC)
{
	//计算电角度
	getCorrectedElectricalAngle(pFOC);
	
	if(1 == pFOC->current.Mflag){
	pFOC->current.ad_A = Motor1_AD_Value[1];
	pFOC->current.ad_B = Motor1_AD_Value[0];
	}
	
	else if(2 == pFOC->current.Mflag){
	pFOC->current.ad_A = Motor2_AD_Value[1];
	pFOC->current.ad_B = Motor2_AD_Value[0];
	}
	
	pFOC->Ia = (pFOC->current.ad_A - pFOC->current.voltage_a_offset)/4096.0f * ADC_REF_VOLTAGE * GAIN;
	pFOC->Ib = (pFOC->current.ad_B - pFOC->current.voltage_a_offset)/4096.0f * ADC_REF_VOLTAGE * GAIN;
	pFOC->Ia = 0 - pFOC->Ia - pFOC->Ib;
	
	clarke_transform(pFOC) ;
	park_transform(pFOC);
	//PID控制器
	pFOC->Ud = PI_Compute(&pi_Id, 0.0f, pFOC->Id);
	pFOC->Uq = PI_Compute(&pi_Id, 0.0f, pFOC->Iq);
	
	pFOC->Ud = 0.0f;
	pFOC->Uq = 6.0f;
	//逆park变换
	inv_park_transform(pFOC);
	SVpwm(PSVpwm_1, pFOC->Ualpha, pFOC->Ubeta);
	//逆clarke变换
	//inv_clarke_transform(pFOC);
	
	//设置PWM
	//setpwm(pFOC);
	setSVpwm(pFOC, PSVpwm_1);
}





