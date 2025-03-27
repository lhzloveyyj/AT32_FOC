#ifndef __FOC_CONFIG_H
#define __FOC_CONFIG_H

#include "at32f403a_407.h"              // Device header

#define VOLTAGE_HIGH 	12.0f 				//电压限制值
#define UDC 			12.0f               //母线电压
#define SQRT3 			1.732f              //根号3
#define POLE_PAIRS 		11                  // 电机的极对数
#define _2PI 			6.28318f            // 2PI
#define _PI 	 		3.14159f            // PI
#define _1_2 	 		0.5f            	// 1/2
#define SQRT3_2			0.866f              //根号3/2
#define _3PI_2			4.712388f           //PI/3
#define EPSILON 		1e-6                // 精度阈值
#define ALL_Duty 		5000            	//time2 PWM慢占空比
#define ADC_REF_VOLTAGE 3.3f                // ADC参考电压
#define RS 				0.01f               //采样电阻值(R)
#define GAIN 			50.0f               //电流放大倍数
#define IQMAX 			40.0f               //最大目标IQ值

#define	Motor_debug	1

#endif
