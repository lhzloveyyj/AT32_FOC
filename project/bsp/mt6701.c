#include "mt6701.h"  

void MT6701_StartRead(MT6701_t *encoder)
{
    /* 拉低 CS */
    gpio_bits_reset(encoder->CS_GPIO, encoder->CS_Pin);
    
    /* 清除标志 */
    encoder->data_ready = 0;
    
    /* 使能 DMA */
    dma_channel_enable(encoder->DMA_TX, TRUE);
    dma_channel_enable(encoder->DMA_RX, TRUE);
    
    /* 启动 SPI 发送 (dummy byte) */
    while (spi_i2s_flag_get(encoder->SPIx, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(encoder->SPIx, 0x00);
}

float MT6701_GetAngle(MT6701_t *encoder)
{
    MT6701_StartRead(encoder);
    
    while (!encoder->data_ready);
    
    float angle = 0.0f;
    if (encoder->data_ready)
    {
        /* 解析角度数据 */
        uint32_t temp = ((encoder->rx_buf[0] << 16) | (encoder->rx_buf[1] << 8) | encoder->rx_buf[2]) >> 10;
        angle = ((float)temp * 360) / 16384.0f;
        angle = angle * 6.28318f / 360.0f;
        
        /* 清除标志 */
        encoder->data_ready = 0;
    }
    
    return angle;
}

MT6701_t mt6701_1 = {SPI1, GPIOA, GPIO_PINS_4, DMA1_CHANNEL4, DMA1_CHANNEL3, {0x00, 0x00, 0x00}, {0}, 0};
MT6701_t mt6701_2 = {SPI2, GPIOB, GPIO_PINS_12, DMA1_CHANNEL6, DMA1_CHANNEL5, {0x00, 0x00, 0x00}, {0}, 0};




