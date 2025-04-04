/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f403a_407_int.c
  * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407_int.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

#include "LED.h"
#include "MT6701.h"
#include "foc.h"
#include "filter.h"
#include "usart_1.h"
#include "speed_control.h"
#include <stdio.h>

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

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
  /* add user code begin NonMaskableInt_IRQ 0 */

  /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */

  /* add user code end NonMaskableInt_IRQ 1 */
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* add user code begin HardFault_IRQ 0 */

  /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
    /* add user code begin W1_HardFault_IRQ 0 */

    /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* add user code begin MemoryManagement_IRQ 0 */

  /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
    /* add user code begin W1_MemoryManagement_IRQ 0 */

    /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* add user code begin BusFault_IRQ 0 */

  /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
    /* add user code begin W1_BusFault_IRQ 0 */

    /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* add user code begin UsageFault_IRQ 0 */

  /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
    /* add user code begin W1_UsageFault_IRQ 0 */

    /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
  /* add user code begin SVCall_IRQ 0 */

  /* add user code end SVCall_IRQ 0 */
  /* add user code begin SVCall_IRQ 1 */

  /* add user code end SVCall_IRQ 1 */
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
  /* add user code begin DebugMonitor_IRQ 0 */

  /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */

  /* add user code end DebugMonitor_IRQ 1 */
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
  /* add user code begin PendSV_IRQ 0 */

  /* add user code end PendSV_IRQ 0 */
  /* add user code begin PendSV_IRQ 1 */

  /* add user code end PendSV_IRQ 1 */
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
  /* add user code begin SysTick_IRQ 0 */

  /* add user code end SysTick_IRQ 0 */


  /* add user code begin SysTick_IRQ 1 */

  /* add user code end SysTick_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 1 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* add user code begin DMA1_Channel1_IRQ 0 */
	if(dma_flag_get(DMA1_FDT1_FLAG) != RESET)
	 {
		dma_flag_clear(DMA1_FDT1_FLAG);
		 
		 FocContorl(PMotor_2, PSVpwm_2, PM2_LPF);
	 }
  /* add user code end DMA1_Channel1_IRQ 0 */
  /* add user code begin DMA1_Channel1_IRQ 1 */

  /* add user code end DMA1_Channel1_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 2 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* add user code begin DMA1_Channel2_IRQ 0 */
	if(dma_flag_get(DMA1_FDT2_FLAG) != RESET)
	 {
		dma_flag_clear(DMA1_FDT2_FLAG);
		 
		 FocContorl(PMotor_1, PSVpwm_1, PM1_LPF);
	 }
  /* add user code end DMA1_Channel2_IRQ 0 */
  /* add user code begin DMA1_Channel2_IRQ 1 */

  /* add user code end DMA1_Channel2_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 3 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* add user code begin DMA1_Channel3_IRQ 0 */
	if(dma_flag_get(DMA1_FDT3_FLAG) != RESET)
	 {
		dma_flag_clear(DMA1_FDT3_FLAG);
	
		 
		dma_channel_enable(DMA1_CHANNEL4, FALSE);
		dma_channel_enable(DMA1_CHANNEL3, FALSE);
		 
		 gpio_bits_set(mt6701_1.CS_GPIO, mt6701_1.CS_Pin);
		 mt6701_1.data_ready = 1;
	 }
  /* add user code end DMA1_Channel3_IRQ 0 */
  /* add user code begin DMA1_Channel3_IRQ 1 */

  /* add user code end DMA1_Channel3_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 5 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* add user code begin DMA1_Channel5_IRQ 0 */
	if(dma_flag_get(DMA1_FDT5_FLAG) != RESET)
	 {
		dma_flag_clear(DMA1_FDT5_FLAG);
		 
		dma_channel_enable(DMA1_CHANNEL6, FALSE);
		dma_channel_enable(DMA1_CHANNEL5, FALSE);
		 
		 gpio_bits_set(mt6701_2.CS_GPIO, mt6701_2.CS_Pin);
		 mt6701_2.data_ready = 1;
	 }
  /* add user code end DMA1_Channel5_IRQ 0 */
  /* add user code begin DMA1_Channel5_IRQ 1 */

  /* add user code end DMA1_Channel5_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 7 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel7_IRQHandler(void)
{
  /* add user code begin DMA1_Channel7_IRQ 0 */
	if(dma_interrupt_flag_get(DMA1_FDT7_FLAG))
  {
    usart1_tx_dma_status = 1;
    dma_flag_clear(DMA1_FDT7_FLAG);
    dma_channel_enable(DMA1_CHANNEL7, FALSE);
  }
  /* add user code end DMA1_Channel7_IRQ 0 */
  /* add user code begin DMA1_Channel7_IRQ 1 */

  /* add user code end DMA1_Channel7_IRQ 1 */
}

/**
  * @brief  this function handles USB Low  Priority or CAN1 RX0 handler.
  * @param  none
  * @retval none
  */
void USBFS_L_CAN1_RX0_IRQHandler(void)
{
  /* add user code begin USBFS_L_CAN1_RX0_IRQ 0 */
	can_rx_message_type rx_message_struct;
	static int times;
	if(can_flag_get(CAN1,CAN_RF0MN_FLAG) != RESET)
	{
		times ++;
		can_message_receive(CAN1, CAN_RX_FIFO0, &rx_message_struct);
		printf("%d	can1 rx data \r\n",times);
		//printf("rx_message_struct.standard_id:%02x \r\n",rx_message_struct.standard_id);
//		if(rx_message_struct.standard_id == FILTER_STD_ID1||rx_message_struct.extended_id==FILTER_EXT_ID1)
//		{
//			printf("rx data:\r\n");
//			for(i=0;i<8;i++)
//			{
//				 printf("%02x ",rx_message_struct.data[i]);
//			}
//			 printf("\r\n");
//		}	
	}

  /* add user code end USBFS_L_CAN1_RX0_IRQ 0 */
  /* add user code begin USBFS_L_CAN1_RX0_IRQ 1 */

  /* add user code end USBFS_L_CAN1_RX0_IRQ 1 */
}

/**
  * @brief  this function handles CAN1 SE handler.
  * @param  none
  * @retval none
  */
void CAN1_SE_IRQHandler(void)
{
  /* add user code begin CAN1_SE_IRQ 0 */

  /* add user code end CAN1_SE_IRQ 0 */
  /* add user code begin CAN1_SE_IRQ 1 */

  /* add user code end CAN1_SE_IRQ 1 */
}

/**
  * @brief  this function handles USART1 handler.
  * @param  none
  * @retval none
  */
void USART1_IRQHandler(void)
{
  /* add user code begin USART1_IRQ 0 */
	if(usart_interrupt_flag_get(USART1, USART_IDLEF_FLAG) != RESET)
	{
		usartdmarecv(uart1_rx_buffer, RX_BUFFER_SIZE);
		rx1_flag = 1;
		usart_flag_clear(USART1, USART_IDLEF_FLAG);
	}
  /* add user code end USART1_IRQ 0 */
  /* add user code begin USART1_IRQ 1 */

  /* add user code end USART1_IRQ 1 */
}

/**
  * @brief  this function handles TMR5 handler.
  * @param  none
  * @retval none
  */
void TMR5_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR5_GLOBAL_IRQ 0 */
	static int t;
	t++;
	if(t==100)
	{
		at32_led_toggle(LED0);
		t=0;
	}
	CalculateSpeed(PMotor_1, 0.001,PM1_LPF_Speed);
	CalculateSpeed(PMotor_2, 0.001,PM2_LPF_Speed);
	
	SpeedPIControl(PMotor_1);
	SpeedPIControl(PMotor_2);
	
	tmr_flag_clear(TMR5, TMR_OVF_FLAG);
  /* add user code end TMR5_GLOBAL_IRQ 0 */


  /* add user code begin TMR5_GLOBAL_IRQ 1 */

  /* add user code end TMR5_GLOBAL_IRQ 1 */
}

/* add user code begin 1 */

/* add user code end 1 */
