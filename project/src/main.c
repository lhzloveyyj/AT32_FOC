/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "wk_adc.h"
#include "wk_can.h"
#include "wk_debug.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

#include "delay.h"
#include "mt6701.h"
#include "FOC.h"
#include "LOG.h"

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define time_pwm	4999
#define	Motor1_debug	1
#define	Motor2_debug	0	
/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* init debug function. */
  wk_debug_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, 
                        (uint32_t)&ADC3->odt, 
                        DMA1_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init dma1 channel2 */
  wk_dma1_channel2_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL2, 
                        (uint32_t)&ADC1->odt, 
                        DMA1_CHANNEL2_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL2_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL2, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init usart3 function. */
  wk_usart3_init();

  /* init spi1 function. */
  wk_spi1_init();

  /* init spi2 function. */
  wk_spi2_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init adc3 function. */
  wk_adc3_init();

  /* init tmr2 function. */
  wk_tmr2_init();

  /* init tmr4 function. */
  wk_tmr4_init();

  /* init tmr5 function. */
  wk_tmr5_init();

  /* init can1 function. */
  wk_can1_init();

  /* init gpio function. */
  wk_gpio_config();

  /* add user code begin 2 */
  delay_init();
  
  Product_Overview();
  
  M1_adc_tigger(time_pwm);
  M2_adc_tigger(time_pwm);
  
  delay_ms(10);
  
  first_get(PMotor_1);
  first_get(PMotor_2);
  
  angle_init(PMotor_1);
  
  angle_init(PMotor_2);
  
  tmr_interrupt_enable(TMR5,TMR_OVF_INT,TRUE);
  
  adc_interrupt_enable(ADC1, ADC_CCE_INT, TRUE);
  adc_interrupt_enable(ADC3, ADC_CCE_INT, TRUE);
  
  //tmr_interrupt_enable(TMR3,TMR_OVF_INT,TRUE);
//  tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, time_pwm * 0.9);
//    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_2, time_pwm * 0.9);
//    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, time_pwm * 0.9);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  printf("motor 1 :ADC:	%d,%d\r\n",Motor1_AD_Value[0],Motor1_AD_Value[1]);
	
	  //机械角度
	  //printf("%lf\r\n",PMotor_2->mechanical_angle);
	  //电角度
	  //printf("%lf,%lf\r\n",PMotor_2->elec_angle,PMotor_2->corr_angle);
	  //校正后的电角度
	  //printf("%lf\r\n",PMotor_2->corr_angle);
	  //三相电压
	  //printf("%lf,%lf,%lf\r\n",PMotor_1->Ua,PMotor_1->Ub,PMotor_1->Uc);
	  
	  //printf("%d,%d\r\n",voltage_a , voltage_b);
	  //printf("%lf,%lf\r\n",Ia,Ib);
	  //printf("%lf,%lf\r\n",Ialpha,Ibeta);
	  //printf("%lf,%lf\r\n",Id,Iq);
	  //printf("%lf\r\n",Ud);
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
