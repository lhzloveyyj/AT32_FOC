#include "mt6701.h"  


uint8_t mt6701_tx_buf[MT6701_BUF_SIZE] = {0x00, 0x00, 0x00};  // SPI 发送数据
uint8_t mt6701_rx_buf[MT6701_BUF_SIZE] = {0};                 // SPI 接收数据

uint8_t mt6701_data_ready;

void MT6701_StartRead(void)
{
    /* 拉低 CS */
    SPI1_CS1_L;  // 片选拉低
    
    /* 清除标志 */
    mt6701_data_ready = 0;

    /* 使能 DMA */
    dma_channel_enable(DMA1_CHANNEL4, TRUE);
    dma_channel_enable(DMA1_CHANNEL3, TRUE);
    
    /* 启动 SPI 发送 (dummy byte) */
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI1, 0x00);
}

float MT6701_GetAngle(void)
{
	MT6701_StartRead();
	
	while(!mt6701_data_ready);
	float angle = 0.0f;
    if(mt6701_data_ready)
    {
        /* 解析角度数据 */
        uint32_t temp = ((mt6701_rx_buf[0] << 16) | (mt6701_rx_buf[1] << 8) | mt6701_rx_buf[2]) >> 10;
		angle = ((float)temp * 360) / 16384.0f;
		angle = angle * 6.28318f / 360.0f;

        /* 清除标志 */
        mt6701_data_ready = 0;
    }

    return angle;
}


