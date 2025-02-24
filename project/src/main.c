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
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

#include "delay.h"
#include "mt6701.h"
#include "FOC.h"

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

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

  /* init tmr3 function. */
  wk_tmr3_init();

  /* init tmr4 function. */
  wk_tmr4_init();

  /* init can1 function. */
  wk_can1_init();

  /* init gpio function. */
  wk_gpio_config();

  /* add user code begin 2 */
  delay_init();
  
  angle_init(MT_2_ReadAngle);
  
  tmr_interrupt_enable(TMR3,TMR_OVF_INT,TRUE);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    //delay_ms(500);
	  //can_transmit_data();
	//float x_2=MT_2_ReadAngle();
	  printf("%lf,%lf,%lf\r\n",Ua,Ub,Uc);
	//float x_1=MT_1_ReadAngle();
	//printf("%lf,%lf\r\n",x_1,x_2);
//	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 1500);
//	tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, 1500);
	//tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, 1500);
	//tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, 1500);
//	  
//	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, 3000);
//	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_2, 3000);
//	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, 3000);
//	tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_4, 3000);
	  //printf("%d\r\n",adc_ordinary_conversion_data_get(ADC1));
	  //printf("%d\r\n",adc_ordinary_conversion_data_get(ADC1));
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
