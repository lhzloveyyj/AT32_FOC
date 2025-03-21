#ifndef __MT6701_H
#define __MT6701_H
#include "at32f403a_407.h"              // Device header

#define SPI1_CS1_L  gpio_bits_reset(GPIOA, GPIO_PINS_4);     //CS1_L
#define SPI1_CS1_H  gpio_bits_set(GPIOA, GPIO_PINS_4);       //CS1_H
#define MT6701_BUF_SIZE   3  // 需要发送和接收的字节数（24bit）

extern uint8_t mt6701_tx_buf[MT6701_BUF_SIZE] ;  // SPI 发送数据
extern uint8_t mt6701_rx_buf[MT6701_BUF_SIZE] ;                 // SPI 接收数据
extern uint8_t mt6701_data_ready;

void MT6701_StartRead(void);
float MT6701_GetAngle(void);

#endif
