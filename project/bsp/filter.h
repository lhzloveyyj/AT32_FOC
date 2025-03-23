#ifndef __FILTER_H
#define __FILTER_H

#include "at32f403a_407.h"  // AT32F403A/407 ͷ�ļ�

// ����Ƶ�� 24kHz, ��ֹƵ�� 500Hz���ʺϵ��ٵ����
#define SAMPLE_FREQ 24000.0f
#define CUTOFF_FREQ 500.0f

#define PI 3.1415f  

/**
 * @brief ��ͨ�˲����ṹ��
 * 
 * ���ڶԵ����źŽ��е�ͨ�˲�
 */
typedef struct {
    float alpha;     ///< �˲�ϵ�����ɲ������ںͽ�ֹƵ�ʼ��㣩
    float filtered;  ///< �˲�������ֵ
} LPF_Filter;

/**
 * @brief  �����˲��ṹ�壬���� Id �� Iq ��
 * 
 * �ýṹ������ͬʱ�� d ��� q ��ĵ������е�ͨ�˲�
 */
typedef struct {
    LPF_Filter Id; ///< d ������˲���
    LPF_Filter Iq; ///< q ������˲���
} LPF_Current;

typedef LPF_Current *PLPF_Current;

// M1����һ�������ĵ����˲���
extern PLPF_Current PM1_LPF;

// M2���ڶ��������ĵ����˲���
extern PLPF_Current PM2_LPF;

/**
 * @brief ��ʼ�������˲���
 * 
 * �ú���������˲�ϵ��������ʼ�� d ��� q ����˲�������
 * 
 * @param Pcurrent ָ����Ҫ��ʼ���� LPF_Current �ṹ��
 */
void LPF_Init(PLPF_Current Pcurrent);

/**
 * @brief ��ͨ�˲����㣨ͬʱ���� d ��� q �������
 * 
 * �ú������ڶ������ d ��� q ����������˲����������˲����ֵ��
 * 
 * @param filter  ָ�� LPF_Current �ṹ���ָ�루���� d ��� q ���˲�����
 * @param Id_input  ��Ҫ�˲��� d ���������
 * @param Iq_input  ��Ҫ�˲��� q ���������
 * @param Id_out   ָ��洢 d ���˲�����ı���
 * @param Iq_out   ָ��洢 q ���˲�����ı���
 */
void LPF_Update(PLPF_Current filter, float Id_input, float Iq_input, float *Id_out, float *Iq_out);

#endif // __FILTER_H
