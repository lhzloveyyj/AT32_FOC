#ifndef __PID_H
#define __PID_H

#include "at32f403a_407.h"              // Device header

// ����PI�������ṹ��
typedef struct {
    float Kp;        // ����ϵ��
    float Ki;        // ����ϵ��
	float error;
	float last_error;
    float integral;  // ������
    float output;    // ���������
    float output_max; // ������ֵ
    float output_min; // �����Сֵ
} PIController;

float PI_Compute(PIController *pi, float target, float current);

#endif
