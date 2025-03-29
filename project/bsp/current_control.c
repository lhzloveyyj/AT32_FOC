#include "current_control.h"
#include "math.h"
#include "stdio.h"

/*************************************************************
** Function name:       CurrentPIControlID
** Descriptions:        D轴电流闭环
** Input parameters:    pFOC:结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void CurrentPIControlID(PFOC_State pFOC)
{
    //获取实际值
    pFOC->idPID.pre = pFOC->Id ;
    //获取目标值
    pFOC->idPID.tar = pFOC->tarid;
    //计算偏差
    pFOC->idPID.bias = pFOC->idPID.tar - pFOC->idPID.pre;
    //计算PID输出值
    pFOC->idPID.out += pFOC->idPID.kp * (pFOC->idPID.bias - pFOC->idPID.lastBias) + pFOC->idPID.ki * pFOC->idPID.bias;
    //保存偏差
    pFOC->idPID.lastBias = pFOC->idPID.bias;

    if (pFOC->idPID.out > fabs(pFOC->idPID.outMax)) {
        pFOC->idPID.out = fabs(pFOC->idPID.outMax);
    }

    if (pFOC->idPID.out < -fabs(pFOC->idPID.outMax)) {
        pFOC->idPID.out = -fabs(pFOC->idPID.outMax);
    }
}
/*************************************************************
** Function name:       CurrentPIControlIQ
** Descriptions:        Q轴电流闭环
** Input parameters:    pFOC:结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void CurrentPIControlIQ(PFOC_State pFOC)
{
    //获取实际值
    pFOC->iqPID.pre = pFOC->Iq;

    //获取目标值
    pFOC->iqPID.tar = pFOC->tariq;

    /*************速度环********************/
    pFOC->iqPID.tar = pFOC->speedPID.out;
    /***************************************/

    //计算偏差
    pFOC->iqPID.bias = pFOC->iqPID.tar - pFOC->iqPID.pre;
    //计算PID输出值
    pFOC->iqPID.out += pFOC->iqPID.kp * (pFOC->iqPID.bias - pFOC->iqPID.lastBias) + pFOC->iqPID.ki * pFOC->iqPID.bias;
    //保存偏差
    pFOC->iqPID.lastBias = pFOC->iqPID.bias;

    if (pFOC->iqPID.out > fabs(pFOC->iqPID.outMax)) {
        pFOC->iqPID.out = fabs(pFOC->iqPID.outMax);
    }

    if (pFOC->iqPID.out < -fabs(pFOC->iqPID.outMax)) {
        pFOC->iqPID.out = -fabs(pFOC->iqPID.outMax);
    }
}


/******************************************************************************
  函数说明：设置电流PID目标值
  @brief  设置电流环PID控制中d轴和q轴的目标值
  @param  pFOC   指向FOC状态结构体的指针
  @param  tarid  d轴电流目标值
  @param  tariq  q轴电流目标值
  @retval 无
******************************************************************************/
void SetCurrentPIDTar(PFOC_State pFOC,float tarid,float tariq)
{
    pFOC->tarid = tarid;
    pFOC->tariq = tariq;
	printf("set tar_id is %lf, tar_iq is %lf\r\n",tarid,tariq);
}

/******************************************************************************
  函数说明：设置电流PID控制参数
  @brief  设置电流环PID控制器的比例、积分、微分系数以及输出限幅
  @param  pFOC   指向FOC状态结构体的指针
  @param  kp     比例系数
  @param  ki     积分系数
  @param  kd     微分系数
  @param  outMax PID输出的最大值
  @retval 无
******************************************************************************/
void SetCurrentPIDParams(PFOC_State pFOC,float kp,float ki,float kd,float outMax)
{
    pFOC->idPID.kp = kp;
    pFOC->idPID.ki = ki;
    pFOC->idPID.kd = kd;
    pFOC->idPID.outMax = outMax;
	
	pFOC->iqPID.kp = kp;
    pFOC->iqPID.ki = ki;
    pFOC->iqPID.kd = kd;
    pFOC->iqPID.outMax = outMax;
	printf("set kp is %lf, ki is %lf, kd is %lf\r\n",kp,ki,kd);
}




