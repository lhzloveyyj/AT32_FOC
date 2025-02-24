/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_usart.c
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
#include "wk_usart.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init usart1 function
  * @param  none
  * @retval none
  */
void wk_usart1_init(void)
{
  /* add user code begin usart1_init 0 */

  /* add user code end usart1_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart1_init 1 */

  /* add user code end usart1_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure param */
  usart_init(USART1, 921600, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_parity_selection_config(USART1, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART1, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart1_init 2 */

  /* add user code end usart1_init 2 */

  usart_enable(USART1, TRUE);

  /* add user code begin usart1_init 3 */

  /* add user code end usart1_init 3 */
}

/**
  * @brief  init usart3 function
  * @param  none
  * @retval none
  */
void wk_usart3_init(void)
{
  /* add user code begin usart3_init 0 */

  /* add user code end usart3_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart3_init 1 */

  /* add user code end usart3_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure param */
  usart_init(USART3, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART3, TRUE);
  usart_receiver_enable(USART3, TRUE);
  usart_parity_selection_config(USART3, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART3, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart3_init 2 */

  /* add user code end usart3_init 2 */

  usart_enable(USART3, TRUE);

  /* add user code begin usart3_init 3 */

  /* add user code end usart3_init 3 */
}

/* add user code begin 1 */

/* add user code end 1 */
