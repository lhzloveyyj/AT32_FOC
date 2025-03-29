#include "speed_control.h"
#include "filter.h"
#include "math.h"
#include "stdio.h"

void CalculateSpeed(PFOC_State pFOC, float dt, PLPF_Speed pSpeedFilter)
{
    float angle_diff = (pFOC->mechanical_angle - pFOC->speed_last_angle);

    // 处理跨 0 和 2π 的跳变
    if (angle_diff > 3.14159f)
        angle_diff -= 6.28318f;
    else if (angle_diff < -3.14159f)
        angle_diff += 6.28318f;

    // 计算原始速度
    pFOC->speed = angle_diff / dt;

    // 使用滤波器平滑速度
    LPF_Speed_Update(pSpeedFilter, pFOC->speed, &(pFOC->speed));

    // 更新上一次的角度值
    pFOC->speed_last_angle = pFOC->mechanical_angle;
}

/*************************************************************
** Function name:       SpeedPIControlID
** Descriptions:        速度闭环
** Input parameters:    pFOC:结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SpeedPIControl(PFOC_State pFOC)
{
    //获取实际值
    pFOC->speedPID.pre = pFOC->speed ;
    //获取目标值
    pFOC->speedPID.tar = pFOC->tar_speed;
    //计算偏差
    pFOC->speedPID.bias = pFOC->speedPID.tar - pFOC->speedPID.pre;
    //计算PID输出值
    pFOC->speedPID.out += pFOC->speedPID.kp * (pFOC->speedPID.bias - pFOC->speedPID.lastBias) + pFOC->speedPID.ki * pFOC->speedPID.bias;
    //保存偏差
    pFOC->speedPID.lastBias = pFOC->speedPID.bias;

    if (pFOC->speedPID.out > fabs(pFOC->speedPID.outMax)) {
        pFOC->speedPID.out = fabs(pFOC->speedPID.outMax);
    }

    if (pFOC->speedPID.out < -fabs(pFOC->speedPID.outMax)) {
        pFOC->speedPID.out = -fabs(pFOC->speedPID.outMax);
    }
}

/******************************************************************************
  函数说明：设置速度PID目标值
  @brief  设置速度环PID控制中速度的目标值
  @param  pFOC   指向FOC状态结构体的指针
  @param  tarspeed  速度目标值
  @retval 无
******************************************************************************/
void SetSpeedPIDTar(PFOC_State pFOC,float tarspeed)
{
    pFOC->tar_speed = tarspeed;
	printf("set tarspeed is %lf\r\n",tarspeed);
}

/******************************************************************************
  函数说明：设置速度PID控制参数
  @brief  设置速度PID控制器的比例、积分、微分系数以及输出限幅
  @param  pFOC   指向FOC状态结构体的指针
  @param  kp     比例系数
  @param  ki     积分系数
  @param  kd     微分系数
  @param  outMax PID输出的最大值
  @retval 无
******************************************************************************/
void SetSpeedPIDParams(PFOC_State pFOC,float kp,float ki,float kd,float outMax)
{
    pFOC->speedPID.kp = kp;
    pFOC->speedPID.ki = ki;
    pFOC->speedPID.kd = kd;
    pFOC->speedPID.outMax = outMax;
printf("set kp is %lf, ki is %lf, kd is %lf\r\n",kp,ki,kd);
}


