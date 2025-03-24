#include "usart_1.h"                  // Device header 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t uart1_tx_buffer[USART1_TX_BUFFER_SIZE] = {0};
volatile uint8_t usart1_tx_dma_status = 0;

uint8_t uart1_rx_buffer[RX_BUFFER_SIZE] = {0};
volatile uint8_t usart1_rx_dma_status = 0;

PID_Params pid_params = {0.0f, 0.0f, 0.0f};

void parse_and_set_pid(const char *input, PID_Params *pid_params);

int fputc(int ch, FILE *f)
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  while(usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}


void USART1_SendFloatArray(float *data, uint8_t size) {

	
    uint8_t *bytePtr = (uint8_t *)data; // �� float ����ת��Ϊ�ֽ�����
    
    // ���� float ���ݵ� yx_buffer
    for (uint8_t i = 0; i < size * 4; i++) {
        uart1_tx_buffer[i] = bytePtr[i];
    }

    // ׷��֡β {0x00, 0x00, 0x80, 0x7F}
    uart1_tx_buffer[size * 4] = 0x00;
    uart1_tx_buffer[size * 4 + 1] = 0x00;
    uart1_tx_buffer[size * 4 + 2] = 0x80;
    uart1_tx_buffer[size * 4 + 3] = 0x7F;
	
	while(usart1_tx_dma_status == 0){};
    // ͳһ�������� buffer
    dma_channel_enable(DMA1_CHANNEL7, TRUE);
}

/**
*	����1��DMA�������ú���
* 	data���������ݵ�buf��ַ
*	len���趨���յ�������ݳ���
*/
 
void usartdmarecv(u8 *data,u16 len)
{
 
    dma_flag_clear(DMA2_FDT1_FLAG);                 //���־  
    dma_channel_enable(DMA2_CHANNEL1, FALSE);       //�ر�USART1 DMA ����
    usart_dma_receiver_enable(USART1,FALSE);        //�ر�ͨ��5
    DMA2_CHANNEL1->dtcnt=RX_BUFFER_SIZE;                       //���յ����ݳ���
    DMA2_CHANNEL1->maddr=(uint32_t)data; //�������buf��ַ
    usart_dma_receiver_enable(USART1,TRUE);         //����USART1 DMA ����
    dma_channel_enable(DMA2_CHANNEL1, TRUE);        //����ͨ��5����ʼ���գ�	
	//printf("%s",data);
	parse_and_set_pid(data, &pid_params);
}

// ���ݲ�ͬ���ƽ�����������Ӧ�� PID ����
void parse_and_set_pid(const char *input, PID_Params *pid_params) {
    // �������Ʋ��֣��� ':' �ָ���
    const char *colon_pos = strchr(input, ':');
    if (colon_pos == NULL) {
        printf("��Ч�������ʽ\n");
        return;
    }

    // ��ȡ���Ʋ��֣��� ':' ǰ�����ݣ�
    size_t name_len = colon_pos - input;
    char name[name_len + 1];
    strncpy(name, input, name_len);
    name[name_len] = '\0';

    // ��ȡ������ֵ���֣��� ':' ������ݣ�
    float value = strtof(colon_pos + 1, NULL);

    // �����������ò�ͬ�� PID ����
    if (strcmp(name, "KP") == 0) {
        pid_params->KP = value;
        printf("set KP: %.2f\n", pid_params->KP);
    } else if (strcmp(name, "KI") == 0) {
        pid_params->KI = value;
        printf("set KI: %.2f\n", pid_params->KI);
    } else if (strcmp(name, "KD") == 0) {
        pid_params->KD = value;
        printf("set KD: %.2f\n", pid_params->KD);
    } else if (strcmp(name, "Iq") == 0) {
        pid_params->Iq = value;
        printf("set LQ: %.2f\n", pid_params->Iq);
    } else if (strcmp(name, "Id") == 0) {
        pid_params->Id = value;
        printf("set Ld: %.2f\n", pid_params->Id);
    } else {
        printf("UNK_name: %s\n", name);
    }
}
