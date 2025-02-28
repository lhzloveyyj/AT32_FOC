#include "pid.h"

// PI控制器计算输出（带抗积分饱和）
float PI_Compute(PIController *pi, float target, float current) {
    // 计算误差
    pi->error = target - current;

    // 计算积分项
    pi->integral = pi->Kp * (pi->error - pi->last_error) + pi->Ki * pi->error;

    // 防止积分项饱和（限幅）
    if (pi->integral > pi->output_max) {
        pi->integral = pi->output_max;
    } else if (pi->integral < pi->output_min) {
        pi->integral = pi->output_min;
    }

    // 计算PI输出
    pi->output += pi->integral;

    // 防止输出饱和
    if (pi->output > pi->output_max) {
        pi->output = pi->output_max;
    } else if (pi->output < pi->output_min) {
        pi->output = pi->output_min;
    }
	
	pi->last_error = pi->error;

    return pi->output;
}
