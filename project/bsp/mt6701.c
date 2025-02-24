#include "mt6701.h"  

#define SPI2_CS1_L  gpio_bits_reset(GPIOB, GPIO_PINS_12);     //CS1_L
#define SPI2_CS1_H  gpio_bits_set(GPIOB, GPIO_PINS_12);       //CS1_H

#define SPI1_CS1_L  gpio_bits_reset(GPIOA, GPIO_PINS_4);     //CS1_L
#define SPI1_CS1_H  gpio_bits_set(GPIOA, GPIO_PINS_4);       //CS1_H

unsigned char SPIx_ReadWriteByte(spi_type* spi_x , unsigned char byte)
{
	unsigned short retry = 0;
	
	while (spi_i2s_flag_get(spi_x, SPI_I2S_TDBE_FLAG) == RESET)
	{
		if(++retry>200)return 0;
	}
	spi_i2s_data_transmit(spi_x, byte);
	retry = 0;
	while (spi_i2s_flag_get(spi_x, SPI_I2S_RDBF_FLAG) == RESET) 
	{
		if(++retry>200)return 0;
	}
	return spi_i2s_data_receive(spi_x);
}

float MT_1_ReadAngle(void)
{
	uint8_t back1,back2,back3;
	//uint8_t MagField;
	uint32_t temp;
	float angle ;
	
	SPI1_CS1_L;
	back1 = SPIx_ReadWriteByte(SPI1 , 0);
	back2 = SPIx_ReadWriteByte(SPI1 , 0);
	back3 = SPIx_ReadWriteByte(SPI1 , 0);
	SPI1_CS1_H;
	
	temp = ((back1<<16)|(back2<<8)|back3)>>10;
	//MagField=(temp>>6)&0x0F;
	angle =((float)temp * 360)/16384.0f;
	angle = angle * 6.28318f / 360.0f;
	
	return angle;
}

float MT_2_ReadAngle(void)
{
	uint8_t back1,back2,back3;
	//uint8_t MagField;
	uint32_t temp;
	float angle ;
	
	SPI2_CS1_L;
	back1 = SPIx_ReadWriteByte(SPI2 , 0);
	back2 = SPIx_ReadWriteByte(SPI2 , 0);
	back3 = SPIx_ReadWriteByte(SPI2 , 0);
	SPI2_CS1_H;
	
	temp = ((back1<<16)|(back2<<8)|back3)>>10;
	//MagField=(temp>>6)&0x0F;
	angle =((float)temp * 360)/16384.0f;
	angle = angle * 6.28318f / 360.0f;
	
	return angle;
}

