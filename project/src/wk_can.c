/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_can.c
  * @brief    work bench config program
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
#include "wk_can.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init can1 function.
  * @param  none
  * @retval none
  */
void wk_can1_init(void)
{
  /* add user code begin can1_init 0 */

  /* add user code end can1_init 0 */
  
  gpio_init_type gpio_init_struct;
  can_base_type can_base_struct;
  can_baudrate_type can_baudrate_struct;
  can_filter_init_type can_filter_init_struct;

  /* add user code begin can1_init 1 */

  /* add user code end can1_init 1 */
  
  /*gpio-----------------------------------------------------------------------------*/ 
  gpio_default_para_init(&gpio_init_struct);

  /* configure the CAN1 TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_12;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the CAN1 RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /*can_base_init--------------------------------------------------------------------*/ 
  can_default_para_init(&can_base_struct);
  can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;
  can_base_struct.ttc_enable = FALSE;
  can_base_struct.aebo_enable = TRUE;
  can_base_struct.aed_enable = TRUE;
  can_base_struct.prsf_enable = FALSE;
  can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
  can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;

  can_base_init(CAN1, &can_base_struct);

  /*can_baudrate_setting-------------------------------------------------------------*/ 
  /*set baudrate = pclk/(baudrate_div *(1 + bts1_size + bts2_size))------------------*/ 
  can_baudrate_struct.baudrate_div = 30;                       /*value: 1~0xFFF*/
  can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;                /*value: 1~4*/
  can_baudrate_struct.bts1_size = CAN_BTS1_6TQ;                /*value: 1~16*/
  can_baudrate_struct.bts2_size = CAN_BTS2_1TQ;                /*value: 1~8*/
  can_baudrate_set(CAN1, &can_baudrate_struct);

  /*can_filter_0_config--------------------------------------------------------------*/
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_number = 0;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
  can_filter_init_struct.filter_bit = CAN_FILTER_16BIT;  
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;  
  /*Standard identifier + Mask Mode + Data/Remote frame: id/mask 11bit --------------*/
  can_filter_init_struct.filter_id_high = 0x0 << 5;
  can_filter_init_struct.filter_id_low = 0x0 << 5;
  can_filter_init_struct.filter_mask_high = 0x0 << 5;
  can_filter_init_struct.filter_mask_low = 0x0 << 5;

  can_filter_init(CAN1, &can_filter_init_struct);

  /*can_filter_1_config--------------------------------------------------------------*/
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_number = 1;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO1;
  can_filter_init_struct.filter_bit = CAN_FILTER_16BIT;  
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;  
  /*Standard identifier + Mask Mode + Data/Remote frame: id/mask 11bit --------------*/
  can_filter_init_struct.filter_id_high = 0x0 << 5;
  can_filter_init_struct.filter_id_low = 0x0 << 5;
  can_filter_init_struct.filter_mask_high = 0x0 << 5;
  can_filter_init_struct.filter_mask_low = 0x0 << 5;

  can_filter_init(CAN1, &can_filter_init_struct);

  /**
   * Users need to configure CAN1 interrupt functions according to the actual application.
   * 1. Call the below function to enable the corresponding CAN1 interrupt.
   *     --can_interrupt_enable(...)
   * 2. Add the user's interrupt handler code into the below function in the at32f403a_407_int.c file.
   *     --void USBFS_L_CAN1_RX0_IRQHandler(void)
   *     --void CAN1_SE_IRQHandler(void)
   */

  /*can1 rx0 interrupt config--------------------------------------------------------*/ 
  //can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, TRUE);

  /*can1 se interrupt config---------------------------------------------------------*/ 
  //can_interrupt_enable(CAN1, CAN_ETRIEN_INT, TRUE);
  //can_interrupt_enable(CAN1, CAN_EOIEN_INT, TRUE);

  /* add user code begin can1_init 2 */
	can_interrupt_enable(CAN1, CAN_RF0MIEN_INT | CAN_RF1MIEN_INT, TRUE);
	can_interrupt_enable(CAN1, CAN_ETRIEN_INT, TRUE);
	can_interrupt_enable(CAN1, CAN_EOIEN_INT, TRUE);
  /* add user code end can1_init 2 */
}

/* add user code begin 1 */

/* add user code end 1 */
