/******************************************************************************
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

#define VOLTAGE_HIGH 	12.0f 				//电压限制值
#define UDC 			12.0f               //母线电压
#define SQRT3 			1.732f              //根号3
#define POLE_PAIRS 		11                  // 电机的极对数
#define _2PI 			6.28318f            // 2PI
#define _PI 	 		3.14159f            // PI
#define SQRT3_2			0.866f              //根号3/2
#define _3PI_2			4.712388f           //PI/3
#define EPSILON 		1e-6                // 精度阈值
#define TIME2_PWM 		6000                //time2 PWM慢占空比
#define ADC_REF_VOLTAGE 3.3f                // ADC参考电压
#define RS 				0.01f               //采样电阻值(R)
#define GAIN 			50.0f               //电流放大倍数
                                            
uint16_t AD_Value[2] = {0};                
uint16_t voltage_a_offset = 0;              	
uint16_t voltage_b_offset = 0; 


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
void adc_tigger(int time_pwm);
void first_get(uint16_t *Ua_offset, uint16_t *Ub_offset);
static void current_transformation(int Va, int Vb, FOC_State *foc);
static void clarke_transform(PFOC_State pFOC) ;
static void park_transform(PFOC_State pFOC);
static void inv_park_transform(PFOC_State pFOC);
static void inv_clarke_transform(PFOC_State pFOC);
static void FocContorl(PFOC_State pFOC);
void setPhaseVoltage(uint16_t V_a, uint16_t V_b, PFOC_State pFOC );

// 定义motor_1
FOC_State Motor_2 = {
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

    .Get_mechanical_angle = MT_2_ReadAngle  
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
		printf("angle_init OK !\r\n");
	}
}

//定时器触发ADC采样
void adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, time_pwm-10);
}

//获取电压偏置
void first_get(uint16_t *Ua_offset,uint16_t *Ub_offset)
{
	for(int i=0;i<16;i++)
	{
		*Ub_offset += AD_Value[0];
		*Ua_offset += AD_Value[1];
//		printf("%d	Ua voltage offset is %d \r\n Ub voltage offset is  %d\r\n",i,AD_Value[1],AD_Value[0]);
	}
	*Ua_offset = *Ua_offset>>4;
	*Ub_offset = *Ub_offset>>4;
	
	printf("Ua voltage offset is %d Ub \r\n voltage offset is  %d\r\n",*Ua_offset,*Ub_offset);
	
}

//电流转换
static void current_transformation(int Va, int Vb, PFOC_State pFOC)
{
	pFOC->Ia = Va/4096.0f * ADC_REF_VOLTAGE * GAIN;
	pFOC->Ib = Vb/4096.0f * ADC_REF_VOLTAGE * GAIN;
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
	
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_a * TIME2_PWM);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * TIME2_PWM);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_c * TIME2_PWM);
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
static void FocContorl(PFOC_State pFOC)
{
	//计算电角度
	getCorrectedElectricalAngle(pFOC);
	clarke_transform(pFOC) ;
	park_transform(pFOC);
	//PID控制器
	pFOC->Ud = PI_Compute(&pi_Id, 0.0f, pFOC->Id);
	pFOC->Uq = PI_Compute(&pi_Id, 0.0f, pFOC->Iq);
	
	pFOC->Ud = 0.0f;
	pFOC->Uq = 2.0f;
	//逆变换
	inv_park_transform(pFOC);
	inv_clarke_transform(pFOC);
	//设置PWM
	setpwm(pFOC);
}

//FOC控制
void setPhaseVoltage(uint16_t V_a, uint16_t V_b, PFOC_State pFOC )
{	
	int voltage_a = V_a - voltage_a_offset;
	int voltage_b = V_b - voltage_b_offset;
	
	current_transformation(voltage_a, voltage_b, pFOC);
	
	FocContorl(pFOC);
}




