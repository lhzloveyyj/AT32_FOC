#ifndef __FILTER_H
#define __FILTER_H

#include "at32f403a_407.h"  // AT32F403A/407 头文件

// 采样频率 24kHz, 截止频率 500Hz（适合低速电机）
#define SAMPLE_FREQ 24000.0f
#define CUTOFF_FREQ 500.0f

#define PI 3.1415f  

/**
 * @brief 低通滤波器结构体
 * 
 * 用于对单个信号进行低通滤波
 */
typedef struct {
    float alpha;     ///< 滤波系数（由采样周期和截止频率计算）
    float filtered;  ///< 滤波后的输出值
} LPF_Filter;

/**
 * @brief  电流滤波结构体，包含 Id 和 Iq 轴
 * 
 * 该结构体用于同时对 d 轴和 q 轴的电流进行低通滤波
 */
typedef struct {
    LPF_Filter Id; ///< d 轴电流滤波器
    LPF_Filter Iq; ///< q 轴电流滤波器
} LPF_Current;

typedef LPF_Current *PLPF_Current;

// M1（第一组电机）的电流滤波器
extern PLPF_Current PM1_LPF;

// M2（第二组电机）的电流滤波器
extern PLPF_Current PM2_LPF;

/**
 * @brief 初始化电流滤波器
 * 
 * 该函数会计算滤波系数，并初始化 d 轴和 q 轴的滤波参数。
 * 
 * @param Pcurrent 指向需要初始化的 LPF_Current 结构体
 */
void LPF_Init(PLPF_Current Pcurrent);

/**
 * @brief 低通滤波计算（同时处理 d 轴和 q 轴电流）
 * 
 * 该函数用于对输入的 d 轴和 q 轴电流进行滤波，并返回滤波后的值。
 * 
 * @param filter  指向 LPF_Current 结构体的指针（包含 d 轴和 q 轴滤波器）
 * @param Id_input  需要滤波的 d 轴电流输入
 * @param Iq_input  需要滤波的 q 轴电流输入
 * @param Id_out   指向存储 d 轴滤波结果的变量
 * @param Iq_out   指向存储 q 轴滤波结果的变量
 */
void LPF_Update(PLPF_Current filter, float Id_input, float Iq_input, float *Id_out, float *Iq_out);



/**
 * @brief 速度滤波结构体
 * 
 * 该结构体用于对速度信号进行低通滤波
 */
typedef struct {
    LPF_Filter speed; ///< 速度滤波器
} LPF_Speed;

typedef LPF_Speed *PLPF_Speed;

// M1（第一组电机）的速度滤波器
extern PLPF_Speed PM1_LPF_Speed;

// M2（第二组电机）的速度滤波器
extern PLPF_Speed PM2_LPF_Speed;

/**
 * @brief 初始化速度滤波器
 * 
 * 该函数会计算滤波系数，并初始化速度滤波参数。
 * 
 * @param Pspeed 指向需要初始化的 LPF_Speed 结构体
 */
void LPF_Speed_Init(PLPF_Speed Pspeed);

/**
 * @brief 低通滤波计算（用于速度信号）
 * 
 * @param filter  指向 LPF_Speed 结构体的指针
 * @param speed_input  需要滤波的速度输入
 * @return 滤波后的速度
 */
void LPF_Speed_Update(PLPF_Speed filter, float speed_input, float *speed_out);

#endif // __FILTER_H
