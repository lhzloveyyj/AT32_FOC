#ifndef __PID_H
#define __PID_H

#include "at32f403a_407.h"              // Device header

// 定义PI控制器结构体
typedef struct {
    float Kp;        // 比例系数
    float Ki;        // 积分系数
	float error;
	float last_error;
    float integral;  // 积分项
    float output;    // 控制器输出
    float output_max; // 输出最大值
    float output_min; // 输出最小值
} PIController;

float PI_Compute(PIController *pi, float target, float current);

#endif
