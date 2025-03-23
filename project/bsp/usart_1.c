#include "usart_1.h"                  // Device header 
#include "stdio.h"

uint8_t uart1_tx_buffer[USART1_TX_BUFFER_SIZE] = {0};
volatile uint8_t usart1_tx_dma_status = 0;

int fputc(int ch, FILE *f)
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  while(usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}

//void USART1_SendFloatArray(float *data, uint8_t size) {
//    uint8_t *bytePtr = (uint8_t *)data; // 将 float 数据转换为字节数组
//    
//    // 复制 float 数据到 yx_buffer
//    for (uint8_t i = 0; i < size * 4; i++) {
//        uart1_tx_buffer[i] = bytePtr[i];
//    }

//    // 追加帧尾 {0x00, 0x00, 0x80, 0x7F}
//    uart1_tx_buffer[size * 4] = 0x00;
//    uart1_tx_buffer[size * 4 + 1] = 0x00;
//    uart1_tx_buffer[size * 4 + 2] = 0x80;
//    uart1_tx_buffer[size * 4 + 3] = 0x7F;

//    // 统一发送整个 buffer
//    for (uint8_t i = 0; i < USART1_TX_BUFFER_SIZE; i++) {
//        while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET) {};
//        usart_data_transmit(USART1, uart1_tx_buffer[i]);
//    }
//}

void USART1_SendFloatArray(float *data, uint8_t size) {

	
    uint8_t *bytePtr = (uint8_t *)data; // 将 float 数据转换为字节数组
    
    // 复制 float 数据到 yx_buffer
    for (uint8_t i = 0; i < size * 4; i++) {
        uart1_tx_buffer[i] = bytePtr[i];
    }

    // 追加帧尾 {0x00, 0x00, 0x80, 0x7F}
    uart1_tx_buffer[size * 4] = 0x00;
    uart1_tx_buffer[size * 4 + 1] = 0x00;
    uart1_tx_buffer[size * 4 + 2] = 0x80;
    uart1_tx_buffer[size * 4 + 3] = 0x7F;
	
	while(usart1_tx_dma_status == 0){};
    // 统一发送整个 buffer
    dma_channel_enable(DMA1_CHANNEL7, TRUE);
}


