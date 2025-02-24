#include "at32f403a_407.h"              // Device header
#include "foc.h"
#include "math.h"
#include "fast_sin.h"
#include "delay.h"
#include "stdio.h"
#include "mt6701.h"

#define volatge_high 	12.0f				//��ѹ����ֵ
#define Udc 			12.0f				//ĸ�ߵ�ѹ
#define sqrt3			1.732f				//����3
#define polePairs 	 	11 					// ����ļ�����
#define _2PI 	 		6.28318f 			// 2PI
#define _PI 	 		3.14159f 			// PI
#define sqrt3_2			0.866f				//����3/2
#define _3PI_2			4.712388f			//PI/3
#define EPSILON 		1e-6 				// ������ֵ

float  zero = 0.0f;	

void setpwm(float Ua,float Ub,float Uc);

//��ֵ���ƺ���
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

// �ѽǶ�ֵ��һ���� [0, 2pi]
float Angle_limit(float angle)
{
    float a = fmod(angle, _2PI); // fmod()�������ڸ�������ȡ������
    return a >= 0.0f ? a : (a + _2PI);
}

// ��Ƕ� = ��е�Ƕ� * ������
float _electricalAngle(float shaft_angle)
{
    return Angle_limit(shaft_angle * polePairs );
}

// ��ȥ���λ����һ��
float getCorrectedElectricalAngle(float shaft_angle)
{
    float elec_angle = _electricalAngle(shaft_angle);
    // ��ȥ���λ
    float corr_angle = elec_angle - zero;
	// ��һ���� [0, 2��)
    corr_angle = Angle_limit(corr_angle);
    
	// ���� 2PI ����
    if (fabs(corr_angle - _2PI) < EPSILON) 
        corr_angle = 0.0f;
	
    return corr_angle;
}

void angle_init(float (*read_angle_func)(void))
{
	setPhaseVoltage(0.0f, 2.0f, 0.0f); // ����ת��
    delay_ms(1000);

    // ��β������ٶ���
    float sum = 0;
    int samples = 10;
    for (int i = 0; i < samples; i++) {
		/**************************************************/
		//BUG : ��ʹ�����printf�����λУ׼��Ϊ0
		/**************************************************/
		printf("%lf\r\n",_electricalAngle(read_angle_func()));
        sum += _electricalAngle(read_angle_func());
        delay_ms(10); // ÿ�β������ 10ms
    }
    zero = sum / samples;

    printf("���λ�Ƕ�: %f,%lf\r\n", zero,getCorrectedElectricalAngle(read_angle_func()));
    printf("��ʼ�����\r\n");
}

float pwm_a=0,pwm_b=0,pwm_c=0;
void setpwm(float Ua,float Ub,float Uc)
{
	//����޷�
	Ua = limit(Ua,0.0f,volatge_high);
	Ub = limit(Ub,0.0f,volatge_high);
	Uc = limit(Uc,0.0f,volatge_high);
	
	//PWM�޷�
	pwm_a = limit(Ua / Udc , 0.0f , 1.0f);
	pwm_b = limit(Ub / Udc , 0.0f , 1.0f);
	pwm_c = limit(Uc / Udc , 0.0f , 1.0f);
	
	//PWMд��
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_a * 6000);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * 6000);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_c * 6000);
}

// FOC���ĺ���������Uq��Ud�͵�Ƕȣ������·PWM
static float Ualpha=0.0f,Ubate=0.0f;
float Ua=0.0f,Ub=0.0f,Uc=0.0f;
void setPhaseVoltage(float Uq, float Ud, float angle_)
{
	//�����޷�
	Uq = limit(Uq,-Udc/2,Udc/2);
	
	//���Ƕ�
	float angle_el = getCorrectedElectricalAngle(angle_);
	
	//park��任
	Ualpha = -Uq * fast_sin(angle_el) + Ud * fast_cos(angle_el);
	Ubate  =  Uq * fast_cos(angle_el) + Ud * fast_sin(angle_el);
	
	//clarke��任
	Ua = Ualpha + Udc/2;
	Ub = (sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	Uc = (-sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	
	setpwm(Ua,Ub,Uc);
}




