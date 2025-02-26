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

#define volatge_high 	12.0f				//��ѹ����ֵ
#define Udc 			12.0f				//ĸ�ߵ�ѹ
#define sqrt3			1.732f				//����3
#define polePairs 	 	11 					// ����ļ�����
#define _2PI 	 		6.28318f 			// 2PI
#define _PI 	 		3.14159f 			// PI
#define sqrt3_2			0.866f				//����3/2
#define _3PI_2			4.712388f			//PI/3
#define EPSILON 		1e-6 				// ������ֵ
#define time2_pwm		6000				//time2 PWM��ռ�ձ�
#define ADC_reference_voltage 3.3f			// ADC�ο���ѹ
#define	Rs				0.01f				//��������ֵ(R)
#define Gain			50.0f					//�����Ŵ���

float  zero = 0.0f;	
uint16_t AD_Value[2]={0};

uint16_t voltage_a_offset = 0;
uint16_t voltage_b_offset = 0;

typedef struct {
    float i_d;  // d�����
    float i_q;  // q�����
    float v_d;  // d���ѹ
    float v_q;  // q���ѹ
    float theta; // ����Ƕ�
    float omega; // ������ٶ�
} MotorState;

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

//Udǿ��
void strong_drag(float Ud)
{
	float Ualpha=0.0f,Ubate=0.0f;
	float Uq = 0.0f;
	//���Ƕ�
	float angle_el = getCorrectedElectricalAngle(0.0f);
	
	//park��任
	Ualpha = -Uq * fast_sin(angle_el) + Ud * fast_cos(angle_el);
	Ubate  =  Uq * fast_cos(angle_el) + Ud * fast_sin(angle_el);
	
	//clarke��任
	Ua = Ualpha + Udc/2;
	Ub = (sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	Uc = (-sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	
	setpwm(Ua,Ub,Uc);
}

//���λУ׼
void angle_init(float (*read_angle_func)(void))
{
	strong_drag(2.0f); // Udǿ��
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

//ADC����
void adc_tigger(int time_pwm)
{
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, time_pwm-10);
}

//�ϵ��ȡƫ�õ�ѹ
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

//��ѹת����������
void current_transformation(int Ua, int Ub, float *Ia, float *Ib)
{
	*Ia = Ua/4096.0f * ADC_reference_voltage * Gain;
	*Ib = Ub/4096.0f * ADC_reference_voltage * Gain;
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
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, pwm_a * time2_pwm);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, pwm_b * time2_pwm);
	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, pwm_c * time2_pwm);
}

// Clarke�任��������
void clarke_transform(float Ia, float Ib, float *Ialpha, float *Ibeta) {
    *Ialpha = Ia;
    *Ibeta = (1 / sqrt3) * (Ia + 2 * Ib);  // Clarke�任��ʽ���������
}

// Park�任��������
void park_transform(float Ialpha, float Ibeta, float angle_el, float *Id, float *Iq) {
    *Id = Ialpha * fast_cos(angle_el) + Ibeta * fast_sin(angle_el);  // Park�任��ʽ
    *Iq = -Ialpha * fast_sin(angle_el) + Ibeta * fast_cos(angle_el);
}

// FOC���ĺ���������Uq��Ud�͵�Ƕȣ������·PWM
float Ualpha=0.0f,Ubate=0.0f;
float Ialpha=0.0f,Ibeta=0.0f;
float Ia = 0.0f, Ib = 0.0f;
float Ua = 0.0f,Ub = 0.0f,Uc = 0.0f;
float Uq=0.0f,Ud=0.0f;
float Iq=0.0f,Id=0.0f;
int voltage_a = 0,voltage_b = 0;
void setPhaseVoltage(uint16_t V_a, uint16_t V_b, float angle)
{
//	//�����޷�
//	Uq = limit(Uq,-Udc/2,Udc/2);
	
	//��ȥƫ�õ�ѹ
	voltage_a = V_a - voltage_a_offset;
	voltage_b = V_b - voltage_b_offset;
	
	//��ѹ�������
	current_transformation(voltage_a, voltage_b, &Ia, &Ib);
	
	//Clarke�任
	clarke_transform(Ia, Ib, &Ialpha, &Ibeta) ;
	
	//���Ƕ�
	float angle_el = getCorrectedElectricalAngle(angle);
	
	//Park�任
	park_transform(Ialpha, Ibeta, angle_el, &Id, &Iq);
	
	
	
	Uq =  2.0f;
	Ud = 0.0f;
	
	//park��任
	Ualpha = -Uq * fast_sin(angle_el) + Ud * fast_cos(angle_el);
	Ubate  =  Uq * fast_cos(angle_el) + Ud * fast_sin(angle_el);
	
	//clarke��任
	Ua = Ualpha + Udc/2;
	Ub = (sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	Uc = (-sqrt3 * Ubate - Ualpha)/2 + Udc/2;
	
	setpwm(Ua,Ub,Uc);
}




