#include "filter.h"

// 创建 M1（第一组电机）的 d 轴和 q 轴电流滤波器
LPF_Current M1_LPF;
PLPF_Current PM1_LPF = &M1_LPF;

// 创建 M2（第二组电机）的 d 轴和 q 轴电流滤波器
LPF_Current M2_LPF;
PLPF_Current PM2_LPF = &M2_LPF;

/**
 * @brief  初始化电流滤波器
 * 
 * 该函数会初始化 d 轴和 q 轴的低通滤波参数。
 * 
 * @param Pcurrent 指向需要初始化的 LPF_Current 结构体
 */
void LPF_Init(PLPF_Current Pcurrent) {
    float dt = 1.0f / SAMPLE_FREQ;  // 计算采样周期 (s)
    float alpha = 1.0f / (1.0f + (1.0f / (2.0f * PI * dt * CUTOFF_FREQ))); // 计算滤波系数

    // 初始化 d 轴滤波器
    Pcurrent->Id.alpha = alpha;
    Pcurrent->Id.filtered = 0.0f;

    // 初始化 q 轴滤波器
    Pcurrent->Iq.alpha = alpha;
    Pcurrent->Iq.filtered = 0.0f;
}

/**
 * @brief  低通滤波计算（同时处理 d 轴和 q 轴电流）
 * 
 * @param filter  指向 LPF_Current 结构体的指针（包含 d 轴和 q 轴滤波器）
 * @param Id_input  需要滤波的 d 轴电流输入
 * @param Iq_input  需要滤波的 q 轴电流输入
 * @param Id_out   指向存储 d 轴滤波结果的变量
 * @param Iq_out   指向存储 q 轴滤波结果的变量
 */
void LPF_Update(PLPF_Current filter, float Id_input, float Iq_input, float *Id_out, float *Iq_out) {
    // 更新 d 轴电流
    filter->Id.filtered += filter->Id.alpha * (Id_input - filter->Id.filtered);
    *Id_out = filter->Id.filtered;
    
    // 更新 q 轴电流
    filter->Iq.filtered += filter->Iq.alpha * (Iq_input - filter->Iq.filtered);
    *Iq_out = filter->Iq.filtered;
}


// M1（第一组电机）的速度滤波器实例
LPF_Speed M1_LPF_Speed;
PLPF_Speed PM1_LPF_Speed = &M1_LPF_Speed;

// M2（第二组电机）的速度滤波器实例
LPF_Speed M2_LPF_Speed;
PLPF_Speed PM2_LPF_Speed = &M2_LPF_Speed;

/**
 * @brief  初始化速度滤波器
 * 
 * 该函数会初始化速度的低通滤波参数。
 * 
 * @param Pspeed 指向需要初始化的 LPF_Speed 结构体
 */
void LPF_Speed_Init(PLPF_Speed Pspeed) {
    float dt = 1.0f / SAMPLE_FREQ;  // 计算采样周期 (s)
    float alpha = 1.0f / (1.0f + (1.0f / (2.0f * PI * dt * CUTOFF_FREQ))); // 计算滤波系数

    // 初始化速度滤波器
    Pspeed->speed.alpha = alpha;
    Pspeed->speed.filtered = 0.0f;
}


/**
 * @brief 低通滤波计算（用于速度信号）
 * 
 * @param filter 指向 LPF_Speed 结构体的指针
 * @param speed_input 需要滤波的速度输入
 * @param speed_out 指向存储滤波后速度的变量
 */
void LPF_Speed_Update(PLPF_Speed filter, float speed_input, float *speed_out)
{
    // 更新速度滤波
    filter->speed.filtered += filter->speed.alpha * (speed_input - filter->speed.filtered);
    *speed_out = filter->speed.filtered;
}
