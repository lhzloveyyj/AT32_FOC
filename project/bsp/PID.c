#include "pid.h"

// PI����������������������ֱ��ͣ�
float PI_Compute(PIController *pi, float target, float current) {
    // �������
    pi->error = target - current;

    // ���������
    pi->integral = pi->Kp * (pi->error - pi->last_error) + pi->Ki * pi->error;

    // ��ֹ������ͣ��޷���
    if (pi->integral > pi->output_max) {
        pi->integral = pi->output_max;
    } else if (pi->integral < pi->output_min) {
        pi->integral = pi->output_min;
    }

    // ����PI���
    pi->output += pi->integral;

    // ��ֹ�������
    if (pi->output > pi->output_max) {
        pi->output = pi->output_max;
    } else if (pi->output < pi->output_min) {
        pi->output = pi->output_min;
    }
	
	pi->last_error = pi->error;

    return pi->output;
}
