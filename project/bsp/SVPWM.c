#include "SVPWM.h"
#include "foc_config.h"

SVpwm_State SVpwm_1 = {
	.Ts = 1.0f	,
	.sector = 0	,
	.Ta = 0.0f	,
	.Tb = 0.0f	,
	.Tc = 0.0f	,
	
	.u1 = 0.0f	,
	.u2 = 0.0f	,
	.u3 = 0.0f	,
	
	.t1 = 0.0f	,
	.t2 = 0.0f	,
	.t3 = 0.0f	,
	.t4 = 0.0f	,
	.t5 = 0.0f	,
	.t6 = 0.0f	,
	.t7 = 0.0f	,
	
	.times = 0	,
};

PSVpwm_State PSVpwm_1  = &SVpwm_1;

SVpwm_State SVpwm_2 = {
	.Ts = 1.0f	,
	.sector = 0	,
	.Ta = 0.0f	,
	.Tb = 0.0f	,
	.Tc = 0.0f	,
	
	.u1 = 0.0f	,
	.u2 = 0.0f	,
	.u3 = 0.0f	,
	
	.t1 = 0.0f	,
	.t2 = 0.0f	,
	.t3 = 0.0f	,
	.t4 = 0.0f	,
	.t5 = 0.0f	,
	.t6 = 0.0f	,
	.t7 = 0.0f	,
	
	.times = 0	,
};

PSVpwm_State PSVpwm_2  = &SVpwm_2;

void SVpwm(PSVpwm_State PSVpwm, float U_alpha, float U_beta)
{
	PSVpwm->K = SQRT3 * PSVpwm->Ts / UDC ;
	PSVpwm->u1 = U_beta * PSVpwm->K;
	PSVpwm->u2 = (SQRT3_2 * U_alpha - _1_2 * U_beta) * PSVpwm->K; // sqrt(3)/2 = 0.8660254
	PSVpwm->u3 = (-SQRT3_2 * U_alpha - _1_2 * U_beta) * PSVpwm->K;
	
	PSVpwm->sector = (PSVpwm->u1 > 0.0f) + ((PSVpwm->u2 > 0.0f) << 1) + ((PSVpwm->u3 > 0.0f) << 2); // sector = A + 2B + 4C
	// 非零矢量和零矢量作用时间的计算
	switch (PSVpwm->sector)
	{
		case 3: // 扇区1
			PSVpwm->t4 = PSVpwm->u2;
			PSVpwm->t6 = PSVpwm->u1;
			PSVpwm->sum = PSVpwm->t4 + PSVpwm->t6;
			if (PSVpwm->sum > PSVpwm->Ts) // 过调制处理
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t4 *= PSVpwm->k_svpwm;
				PSVpwm->t6 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t4 - PSVpwm->t6) / 2.0f;
			PSVpwm->Ta = PSVpwm->t4 + PSVpwm->t6 + PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t6 + PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t7;
			break;
		case 1: // 扇区2
			PSVpwm->t2 = -PSVpwm->u2;
			PSVpwm->t6 = -PSVpwm->u3;
			PSVpwm->sum = PSVpwm->t2 + PSVpwm->t6;
			if (PSVpwm->sum > PSVpwm->Ts)
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t2 *= PSVpwm->k_svpwm;
				PSVpwm->t6 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t2 - PSVpwm->t6) / 2.0f;
			PSVpwm->Ta = PSVpwm->t6 + PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t2 + PSVpwm->t6 + PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t7;
			break;
		case 5: // 扇区3
			PSVpwm->t2 = PSVpwm->u1;
			PSVpwm->t3 = PSVpwm->u3;
			PSVpwm->sum = PSVpwm->t2 + PSVpwm->t3;
			if (PSVpwm->sum > PSVpwm->Ts)
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t2 *= PSVpwm->k_svpwm;
				PSVpwm->t3 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t2 - PSVpwm->t3) / 2.0f;
			PSVpwm->Ta = PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t2 + PSVpwm->t3 + PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t3 + PSVpwm->t7;
			break;
		case 4: // 扇区4
			PSVpwm->t1 = -PSVpwm->u1;
			PSVpwm->t3 = -PSVpwm->u2;
			PSVpwm->sum = PSVpwm->t1 + PSVpwm->t3;
			if (PSVpwm->sum > PSVpwm->Ts)
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t1 *= PSVpwm->k_svpwm;
				PSVpwm->t3 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t1 - PSVpwm->t3) / 2.0f;
			PSVpwm->Ta = PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t3 + PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t1 + PSVpwm->t3 + PSVpwm->t7;
			break;
		case 6: // 扇区5
			PSVpwm->t1 = PSVpwm->u3;
			PSVpwm->t5 = PSVpwm->u2;
			PSVpwm->sum = PSVpwm->t1 + PSVpwm->t5;
			if (PSVpwm->sum > PSVpwm->Ts)
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t1 *= PSVpwm->k_svpwm;
				PSVpwm->t5 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t1 - PSVpwm->t5) / 2.0f;
			PSVpwm->Ta = PSVpwm->t5 + PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t1 + PSVpwm->t5 + PSVpwm->t7;
			break;
		case 2: // 扇区6
			PSVpwm->t4 = -PSVpwm->u3;
			PSVpwm->t5 = -PSVpwm->u1;
			PSVpwm->sum = PSVpwm->t4 + PSVpwm->t5;
			if (PSVpwm->sum > PSVpwm->Ts)
			{
				PSVpwm->k_svpwm = PSVpwm->Ts / PSVpwm->sum;
				PSVpwm->t4 *= PSVpwm->k_svpwm;
				PSVpwm->t5 *= PSVpwm->k_svpwm;
			}
			PSVpwm->t7 = (PSVpwm->Ts - PSVpwm->t4 - PSVpwm->t5) / 2.0f;
			PSVpwm->Ta = PSVpwm->t4 + PSVpwm->t5 + PSVpwm->t7;
			PSVpwm->Tb = PSVpwm->t7;
			PSVpwm->Tc = PSVpwm->t5 + PSVpwm->t7;
			break;
		default:
			break;
	}
}   
    



