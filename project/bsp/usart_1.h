#ifndef __USART_1_H
#define __USART_1_H

#include "at32f403a_407.h"  
#include "at32f403a_407_usart.h"

#define N								 3
#define USART1_TX_BUFFER_SIZE            (4 * N + 4)

#define RX_BUFFER_SIZE  64  // DMA���ջ�������С

#define PRINT_UART                       USART1
#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_9
#define PRINT_UART_TX_GPIO               GPIOA
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK

// ����ṹ�����洢 PID ����
typedef struct {
    float KP;
    float KI;
    float KD;
	float Iq;  // ������ LQ ����
    float Id;  // ������ Ld ����
} PID_Params;

extern PID_Params pid_params;

void USART1_SendFloatArray(float *data, uint8_t size);
void USART1_SendSinWaveData(float *data);
void ProcessCommand(char *cmd);
void usartdmarecv(u8 *data,u16 len);

extern uint8_t uart1_tx_buffer[USART1_TX_BUFFER_SIZE] ;
extern volatile uint8_t usart1_tx_dma_status;

extern uint8_t uart1_rx_buffer[RX_BUFFER_SIZE] ;
extern volatile uint8_t usart1_rx_dma_status;

#endif
