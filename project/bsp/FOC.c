#include "at32f403a_407.h"              // Device header
#include "foc.h"
#include "math.h"
#include "fast_sin.h"
#include "delay.h"
#include "stdio.h"
#include "mt6701.h"

#define volatge_high 	12.0f				//电压限制值
#define Udc 			12.0f				//母线电压
#define sqrt3			1.732f				//根号3
#define polePairs 	 	11 					// 电机的极对数
#define _2PI 	 		6.28318f 			// 2PI
#define _PI 	 		3.14159f 			// PI
#define sqrt3_2			0.866f				//根号3/2
#define _3PI_2			4.712388f			//PI/3
#define EPSILON 		1e-6 				// 精度阈值

float  zero = 0.0f;	

void setpwm(float Ua,float Ub,float Uc);

//幅值限制函数
float limit(float in_vo,float low,float high)
{
	if(in_vo>=high)
		in_vo=high;
	else if(in_vo<=low)
		in_vo=low;
	else
		in_vo=in_vo;
	return in_vo;
}

// 把角度值归一化在 [0, 2pi]
float Angle_limit(float angle)
{
    float a = fmod(angle, _2PI); // fmod()函数用于浮点数的取余运算
    return a >= 0.0f ? a : (a + _2PI);
}

// 电角度 = 机械角度 * 极对数
float _electricalAngle(float shaft_angle)
{
    return Angle_limit(shaft_angle * polePairs );
}

// 减去零电位并归一化
float getCorrectedElectricalAngle(float shaft_angle)
{
    float elec_angle = _electricalAngle(shaft_angle);
    // 减去零电位
    float corr_angle = elec_angle - zero;
	// 归一化到 [0, 2π)
    corr_angle = Angle_limit(corr_angle);
    
	// 修正 2PI 问题
    if (fabs(corr_angle - _2PI) < EPSILON) 
        corr_angle = 0.0f;
	
    return corr_angle;
}

void angle_init(float (*read_angle_func)(void))
{
	setPhaseVoltage(0.0f, 2.0f, 0.0f); // 锁定转子
    delay_ms(1000);

    // 多次采样减少抖动
    float sum = 0;
    int samples = 10;
    for (int i = 0; i < samples; i++) {
		/**************************************************/
		//BUG : 不使用这个printf则零电位校准后不为0
		/**************************************************/
		printf("%lf\r\n",_electricalAngle(read_angle_func()));
        sum += _electricalAngle(read_angle_func());
        delay_ms(10); // 每次采样间隔 10ms
    }
    zero = sum / samples;

    printf("零电位角度: %f,%lf\r\n", zero,getCorrectedElectricalAngle(read_angle_func()));
    printf("初始化完成\r\n");
}

float pwm_a=0,pwm_b=0,pwm_c=0;
void setpwm(float Ua,float Ub,float Uc)
{
	//输出限幅
	Ua = limit(Ua,0.0f,volatge_high);
	Ub = limit(Ub,0.0f,volatge_high);
	Uc = limit(Uc,0.0f,volatge_high);
	
	//PWM限幅
	pwm_a = limit(Ua / Udc , 0.0f , 1.0f);
	pwm_b = limit(Ub / Udc , 0.0f , 1.0f);
	pwm_c = limit(Uc / Udc , 0.0f , 1.0f);
	
	//PWM写入
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_a * 6000);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * 6000);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_c * 6000);
}

// FOC核心函数：输入Uq、Ud和电角度，输出三路PWM
static float Ualpha=0.0f,Ubate=0.0f;
float Ua=0.0f,Ub=0.0f,Uc=0.0f;
void setPhaseVoltage(float Uq, float Ud, float angle_)
{
	//力矩限幅
	Uq = limit(Uq,-Udc/2,Udc/2);
	
	//求电角度
	float angle_el = getCorrectedElectricalAngle(angle_);
	
	//park逆变换
	Ualpha = -Uq * fast_sin(angle_el) + Ud * fast_cos(angle_el);
	Ubate  =  Uq * fast_cos(angle_el) + Ud * fast_sin(angle_el);
	
	//clarke逆变换
	Ua = Ualpha + Udc/2;
	Ub = (sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	Uc = (-sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	
	setpwm(Ua,Ub,Uc);
}




