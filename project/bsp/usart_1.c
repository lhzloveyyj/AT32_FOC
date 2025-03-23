#include "usart_1.h"                  // Device header 
#include "stdio.h"

int fputc(int ch, FILE *f)
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  while(usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}

void USART1_SendFloatArray(float *data, uint8_t size) {
    uint8_t *bytePtr = (uint8_t *)data; // �� float ����ת��Ϊ�ֽ�����
    for (uint8_t i = 0; i < size * 4; i++) {
		while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET){};
        usart_data_transmit(USART1, bytePtr[i]); // ���ֽڷ���
    }
}

void USART1_SendSinWaveData(float *data) {
    // ���� 4 �� float
    USART1_SendFloatArray(data, 4);

    // ����֡β {0x00, 0x00, 0x80, 0x7F}
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7F};
    for (uint8_t i = 0; i < 4; i++) {
        while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET) {};
        usart_data_transmit(USART1, tail[i]);
    }
}

