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
#include "current_control.h"
#include "foc_config.h"
#include "SVpwm.h"
#include "filter.h"
                                            
uint16_t Motor1_AD_Value[2] = {0};
uint16_t Motor2_AD_Value[2] = {0};

// 函数原型声明
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
void FocContorl(PFOC_State pFOC, PSVpwm_State PSVpwm, PLPF_Current LPF_M);
static void setpwm1_channel(float pwm_a, float pwm_b, float pwm_c);
static void setpwm2_channel(float pwm_a, float pwm_b, float pwm_c);


/******************************************************************************
  定义电机1的FOC状态结构体
******************************************************************************/
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
    .Ia = 0.0f, .Ib = 0.0f, .Ic = 0.0f,			
    .Ua = 0.0f, .Ub = 0.0f, .Uc = 0.0f, 		
    .Uq = 0.0f, .Ud = 0.0f, 			
    .Iq = 0.0f, .Id = 0.0f, 			
    .mechanical_angle = 0.0f, 
    .elec_angle = 0.0f,       
    .corr_angle = 0.0f,       
    .zero = 0.0f, 			

	.idPID = {0},                           
    .iqPID = {0},
	
	.speed_last_angle = 0.0f,
	.speed = 0.0f,
	.speedPID = {0},
	
    .Get_mechanical_angle = MT6701_GetAngle ,
	.SetPWM = setpwm1_channel	,
};

PFOC_State PMotor_1 = &Motor_1;

/******************************************************************************
  定义电机2的FOC状态结构体
******************************************************************************/
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
    .Ia = 0.0f, .Ib = 0.0f, .Ic = 0.0f,			
    .Ua = 0.0f, .Ub = 0.0f, .Uc = 0.0f, 		
    .Uq = 0.0f, .Ud = 0.0f, 			
    .Iq = 0.0f, .Id = 0.0f, 			
    .mechanical_angle = 0.0f, 
    .elec_angle = 0.0f,       
    .corr_angle = 0.0f,       
    .zero = 0.0f, 	

	.idPID = {0},                           
    .iqPID = {0},
	
	.speed_last_angle = 0.0f,
	.speed = 0.0f,	
	.speedPID = {0},

    .Get_mechanical_angle = MT6701_GetAngle ,
	.SetPWM = setpwm2_channel
};

PFOC_State PMotor_2 = &Motor_2;

/******************************************************************************
  函数说明：限幅函数
  @brief  将输入值限制在指定的最小值和最大值之间
  @param  in_vo 输入值
  @param  low   最小限制值
  @param  high  最大限制值
  @return 限幅后的值
******************************************************************************/
static float limit(float in_vo, float low, float high) {
    if (in_vo >= high) return high;
    if (in_vo <= low) return low;
    return in_vo;
}

/******************************************************************************
  函数说明：角度限幅函数
  @brief  将输入角度归一化到 [0, 2π] 区间
  @param  angle 输入角度（单位：弧度）
  @return 归一化后的角度
******************************************************************************/
static float Angle_limit(float angle) {
    float a = fmod(angle, _2PI);
    return a >= 0.0f ? a : (a + _2PI);
}

/******************************************************************************
  函数说明：计算电角度
  @brief  根据机械角和极对数计算电角度
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void _electricalAngle(PFOC_State pFOC) {
    if(1 == pFOC->current.Mflag)
    pFOC->mechanical_angle = pFOC->Get_mechanical_angle(&mt6701_1);
	else if(2 == pFOC->current.Mflag)
    pFOC->mechanical_angle = pFOC->Get_mechanical_angle(&mt6701_2);
    pFOC->elec_angle = Angle_limit(pFOC->mechanical_angle * POLE_PAIRS);
}

/******************************************************************************
  函数说明：获取校正后的电角度
  @brief  计算电角度后减去零点偏移，并归一化
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void getCorrectedElectricalAngle(PFOC_State pFOC) 
{
    _electricalAngle(pFOC);
    pFOC->corr_angle = Angle_limit(pFOC->elec_angle - pFOC->zero);
    if (fabs(pFOC->corr_angle - _2PI) < EPSILON) pFOC->corr_angle = 0.0f;
}

/******************************************************************************
  函数说明：强拖电角度计算
  @brief  在强拖模式下，将机械角设为零，并计算电角度及校正角度
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void strong_drag_Angle(PFOC_State pFOC)
{
    pFOC->mechanical_angle = 0.0f;
	pFOC->elec_angle = Angle_limit(pFOC->mechanical_angle * POLE_PAIRS);
	pFOC->corr_angle = Angle_limit(pFOC->elec_angle - pFOC->zero);
	if (fabs(pFOC->corr_angle - _2PI) < EPSILON) pFOC->corr_angle = 0.0f;
}


/******************************************************************************
  函数说明：强拖模式
  @brief  在强拖模式下输出固定电压，并调用逆变换与PWM设置函数
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void strong_drag(PFOC_State pFOC) {
    
	pFOC->Ud = 2.0f;
	pFOC->Uq = 0.0f;

	
    strong_drag_Angle(pFOC);

    
    inv_park_transform(pFOC);

    
    inv_clarke_transform(pFOC);
    
    setpwm(pFOC);
}

/******************************************************************************
  函数说明：电机角度初始化
  @brief  通过强拖模式初始化电角度，并计算零点偏移
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
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

/******************************************************************************
  函数说明：电机1 ADC触发函数
  @brief  通过定时器通道触发ADC采样（电机1）
  @param  time_pwm PWM周期时间
  @retval 无
******************************************************************************/
void M1_adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_4, time_pwm * 0.9f);
}

/******************************************************************************
  函数说明：电机2 ADC触发函数
  @brief  通过定时器通道触发ADC采样（电机2）
  @param  time_pwm PWM周期时间
  @retval 无
******************************************************************************/
void M2_adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, time_pwm * 0.9f);
}

/******************************************************************************
  函数说明：获取电压偏置
  @brief  通过多次采样计算电压偏置值，用于后续电流测量补偿
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
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

/******************************************************************************
  函数说明：电机1 PWM输出设置函数
  @brief  根据输入占空比设置电机1三相PWM输出
  @param  pwm_a 相A占空比
  @param  pwm_b 相B占空比
  @param  pwm_c 相C占空比
  @retval 无
******************************************************************************/
static void setpwm1_channel(float pwm_a, float pwm_b, float pwm_c)
{
	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, pwm_a * ALL_Duty * 0.9f);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_2, pwm_b * ALL_Duty * 0.9f);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, pwm_c * ALL_Duty * 0.9f);
}

/******************************************************************************
  函数说明：电机2 PWM输出设置函数
  @brief  根据输入占空比设置电机2三相PWM输出
  @param  pwm_a 相A占空比
  @param  pwm_b 相B占空比
  @param  pwm_c 相C占空比
  @retval 无
******************************************************************************/
static void setpwm2_channel(float pwm_a, float pwm_b, float pwm_c)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_a * ALL_Duty * 0.9f);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * ALL_Duty * 0.9f);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_c * ALL_Duty * 0.9f);
}
/******************************************************************************
  函数说明：设置PWM输出
  @brief  根据计算得到的三相电压，对电压进行限幅，并转换为PWM占空比，然后调用PWM设置函数
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
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

/******************************************************************************
  函数说明：设置SVPWM输出
  @brief  根据SVPWM算法计算结果设置PWM输出
  @param  pFOC 指向FOC状态结构体的指针
  @param  PSVpwm 指向SVPWM状态结构体的指针
  @retval 无
******************************************************************************/
static void setSVpwm(PFOC_State pFOC, PSVpwm_State PSVpwm)
{
	
    pFOC->SetPWM(PSVpwm->Ta, PSVpwm->Tb, PSVpwm->Tc);
}

/******************************************************************************
  函数说明：Clarke变换
  @brief  将三相电流转换为αβ坐标系（注意：取反处理确保电流方向一致）
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void clarke_transform(PFOC_State pFOC) {
	//Uq给正时Iq为负，因此对电流取反，保证一致
	pFOC->Ia = -pFOC->Ia;
	pFOC->Ib = -pFOC->Ib;
	
    pFOC->Ialpha = pFOC->Ia;
    pFOC->Ibeta = (pFOC->Ia + 2 * pFOC->Ib) / SQRT3;
}

/******************************************************************************
  函数说明：Park变换
  @brief  将αβ坐标系的电流转换为dq坐标系，以便于电流环控制
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void park_transform(PFOC_State pFOC) {
    pFOC->Id = pFOC->Ialpha * fast_cos(pFOC->corr_angle) + pFOC->Ibeta * fast_sin(pFOC->corr_angle);  
    pFOC->Iq = -pFOC->Ialpha * fast_sin(pFOC->corr_angle) + pFOC->Ibeta * fast_cos(pFOC->corr_angle);
}

/******************************************************************************
  函数说明：逆Park变换
  @brief  将dq坐标系的电压转换为αβ坐标系，以便进行SVPWM计算
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void inv_park_transform(PFOC_State pFOC)
{
	pFOC->Ualpha = -pFOC->Uq * fast_sin(pFOC->corr_angle) + pFOC->Ud * fast_cos(pFOC->corr_angle);
	pFOC->Ubeta  =  pFOC->Uq * fast_cos(pFOC->corr_angle) + pFOC->Ud * fast_sin(pFOC->corr_angle);
}

/******************************************************************************
  函数说明：逆Clarke变换
  @brief  将αβ坐标系的电压转换为三相电压，适用于PWM输出
  @param  pFOC 指向FOC状态结构体的指针
  @retval 无
******************************************************************************/
static void inv_clarke_transform(PFOC_State pFOC)
{
	pFOC->Ua = pFOC->Ualpha + UDC/2;
	pFOC->Ub = (SQRT3 * pFOC->Ubeta - pFOC->Ualpha)/2 + UDC/2;
	pFOC->Uc = (-SQRT3 * pFOC->Ubeta - pFOC->Ualpha)/2 + UDC/2;
}

/******************************************************************************
  函数说明：FOC控制主函数
  @brief  完成FOC控制流程：计算电角度、采样电流、Clarke/Park变换、低通滤波、PID控制、逆变换、SVPWM计算及PWM输出
  @param  pFOC   指向FOC状态结构体的指针
  @param  PSVpwm 指向SVPWM状态结构体的指针
  @param  LPF_M  指向电流低通滤波器结构体的指针
  @retval 无
******************************************************************************/
void FocContorl(PFOC_State pFOC, PSVpwm_State PSVpwm, PLPF_Current LPF_M)
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
	pFOC->Ic = 0 - pFOC->Ia - pFOC->Ib;
	
	clarke_transform(pFOC) ;
	park_transform(pFOC);
	
	//ID,IQ滤波
	LPF_Update(LPF_M, pFOC->Id, pFOC->Iq, &(pFOC->Id), &(pFOC->Iq));
	
	//PID控制器
	CurrentPIControlID(pFOC);
    CurrentPIControlIQ(pFOC);
	
	pFOC->Ud = pFOC->idPID.out;
	pFOC->Uq = pFOC->iqPID.out;
	
	//pFOC->Ud = 1.0f;
	//pFOC->Uq = 0.0f;
	//逆park变换
	inv_park_transform(pFOC);
    
	SVpwm(PSVpwm, pFOC->Ualpha, pFOC->Ubeta);
	
	//设置SVPWM
	setSVpwm(pFOC, PSVpwm);
}



