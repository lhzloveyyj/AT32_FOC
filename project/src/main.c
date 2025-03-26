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
#include "CAN.h"
#include "SVPWM.h"
#include "foc_config.h"
#include "fast_sin.h"
#include "usart_1.h"
#include "filter.h"

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

  /* init dma1 channel3 */
  wk_dma1_channel3_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL3, 
                        (uint32_t)&SPI1->dt, 
                        DMA1_CHANNEL3_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL3_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL3, TRUE);

  /* init dma1 channel4 */
  wk_dma1_channel4_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL4, 
                        (uint32_t)&SPI1->dt, 
                        DMA1_CHANNEL4_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL4_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);

  /* init dma1 channel5 */
  wk_dma1_channel5_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL5, 
                        (uint32_t)&SPI2->dt, 
                        DMA1_CHANNEL5_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL5_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL5, TRUE);

  /* init dma1 channel6 */
  wk_dma1_channel6_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL6, 
                        (uint32_t)&SPI2->dt, 
                        DMA1_CHANNEL6_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL6_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL6, TRUE);

  /* init dma1 channel7 */
  wk_dma1_channel7_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL7, 
                        (uint32_t)&USART1->dt, 
                        DMA1_CHANNEL7_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL7_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL7, TRUE);

  /* init dma2 channel1 */
  wk_dma2_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA2_CHANNEL1, 
                        (uint32_t)&USART1->dt, 
                        DMA2_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA2_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA2_CHANNEL1, TRUE);

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
  
  //USART1_TX_DMA
  dma_interrupt_enable(DMA1_CHANNEL7, DMA_FDT_INT, TRUE);
  //USART_1_RX
  usart_interrupt_enable(USART1, USART_IDLE_INT, TRUE);
  
  Product_Overview();
  
  M1_adc_tigger(ALL_Duty);
  M2_adc_tigger(ALL_Duty);
  
  delay_ms(10);
  
  first_get(PMotor_1);
  first_get(PMotor_2);
  
  //低通滤波器初始化
  LPF_Init(PM1_LPF);
  LPF_Init(PM2_LPF);
  
  //初始化PID参数
  SetCurrentPIDTar(PMotor_1, 0.0f, 0.0f);
  SetCurrentPIDParams(PMotor_1, 0.01f, 0.01f, 0.0f, 6.0f);
  SetCurrentPIDTar(PMotor_2, 0.0f, 0.0f);
  SetCurrentPIDParams(PMotor_2, 0.01f, 0.01f, 0.0f, 6.0f);
  
  //SPI1_DMA
  dma_interrupt_enable(DMA1_CHANNEL3, DMA_FDT_INT, TRUE );
  //SPI2_DMA
  dma_interrupt_enable(DMA1_CHANNEL5, DMA_FDT_INT, TRUE );
  
  angle_init(PMotor_1);
  angle_init(PMotor_2);
  
  tmr_interrupt_enable(TMR5,TMR_OVF_INT,TRUE);
  
  //ADC1_DMA
  dma_interrupt_enable(DMA1_CHANNEL2, DMA_FDT_INT, TRUE);
  //ADC3_DMA
  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */

	  //can1_transmit_data_sid();
	  //delay_ms(300);
	  #if Motor_debug == 1
	  float speed_angle[3] = {PMotor_1->mechanical_angle , PMotor_1->speed_last_angle, PMotor_1->speed};
	  //float Tabc[3] = {PSVpwm_1->Ta,PSVpwm_1->Tb,PSVpwm_1->Tc};
	  //float Iabc[3] = {PMotor_1->Ia, PMotor_1->Ib, 1 - PMotor_1->Ia - PMotor_1->Ib};
	  //float Ialpha_Ibeta[2] = {PMotor_1->Ialpha, PMotor_1->Ibeta};
	  float Iqd[2] = {PMotor_1->Id, PMotor_1->Iq};
	  float speed[1] = {PMotor_1->speed};
	  
	  USART1_SendFloatArray(speed_angle,3);
	  if(pid_params_1.set_flag == 0){
		SetCurrentPIDTar(PMotor_1, pid_params_1.Id, pid_params_1.Iq);
		  pid_params_1.set_flag = 1;
	  }
	  
	  if(pid_params_2.set_flag == 0){
		SetCurrentPIDTar(PMotor_2, pid_params_2.Id, pid_params_2.Iq);
		  pid_params_2.set_flag = 1;
	  }
	  
	  if(rx1_flag == 1){
		parse_and_set_pid((const char *)uart1_rx_buffer, &pid_params_1, &pid_params_2);
		  rx1_flag = 0;
	  }
		
	  
	  #endif
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
