#ifndef __MT6701_H
#define __MT6701_H
#include "at32f403a_407.h"              // Device header

#define MT6701_BUF_SIZE	3

typedef struct {
    spi_type *SPIx;       // SPI 端口
    gpio_type *CS_GPIO;   // 片选 GPIO 端口
    uint16_t CS_Pin;         // 片选 GPIO 引脚
    dma_channel_type *DMA_TX; // 发送 DMA 通道
    dma_channel_type *DMA_RX; // 接收 DMA 通道
    uint8_t tx_buf[MT6701_BUF_SIZE]; // 发送缓冲区
    uint8_t rx_buf[MT6701_BUF_SIZE]; // 接收缓冲区
    volatile uint8_t data_ready;     // 数据准备标志
} MT6701_t;

extern MT6701_t mt6701_1;
extern MT6701_t mt6701_2;

float MT6701_GetAngle(MT6701_t *encoder);

#endif
