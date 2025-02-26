#include "filter.h"

/*
 * @brief  低通滤波器
 * @param  in :输入值
 * @param  out; 滤波之后的值
 * @param  dt: 采样周期
 * @param  fc: 截止频率
 * @retval None
 */
void  LowPassFilter(float in, float *out, float dt, float fc) {
    *out += (1.0f / (1.0f + 1.0f/(2.0f * PI * dt * fc )))*(in - *out);
}





