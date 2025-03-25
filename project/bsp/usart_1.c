#include "usart_1.h"                  // Device header 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t uart1_tx_buffer[USART1_TX_BUFFER_SIZE] = {0};
volatile uint8_t usart1_tx_dma_status = 0;

uint8_t uart1_rx_buffer[RX_BUFFER_SIZE] = {0};
volatile uint8_t usart1_rx_dma_status = 0;

volatile uint8_t rx1_flag = 0;

PID_Params pid_params_1 = {0.0f, 0.0f, 0.0f};
PID_Params pid_params_2 = {0.0f, 0.0f, 0.0f};

void parse_and_set_pid(const char *input, PID_Params *pid_params_1, PID_Params *pid_params_2);

int fputc(int ch, FILE *f)
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  while(usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}


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

/**
*	串口1的DMA接收设置函数
* 	data：接收数据的buf地址
*	len：设定接收的最大数据长度
*/
 
void usartdmarecv(u8 *data,u16 len)
{
 
    dma_flag_clear(DMA2_FDT1_FLAG);                 //清标志  
	usart_dma_receiver_enable(USART1,FALSE);        //关闭通道5
    dma_channel_enable(DMA2_CHANNEL1, FALSE);       //关闭USART1 DMA 接收
    DMA2_CHANNEL1->dtcnt=RX_BUFFER_SIZE;                       //接收的数据长度
    DMA2_CHANNEL1->maddr=(uint32_t)data; //存放数据buf地址
    usart_dma_receiver_enable(USART1,TRUE);         //开启USART1 DMA 接收
    dma_channel_enable(DMA2_CHANNEL1, TRUE);        //开启通道5（开始接收）	
}

void parse_and_set_pid(const char *input, PID_Params *pid_params_1, PID_Params *pid_params_2) {
    // 查找名称部分（以 ':' 分隔）
    const char *colon_pos = strchr(input, ':');
    if (colon_pos == NULL) {
        printf("无效的输入格式\n");
        return;
    }

    // 提取名称部分（即 ':' 前的内容）
    size_t name_len = colon_pos - input;
    char name[name_len + 1];
    strncpy(name, input, name_len);
    name[name_len] = '\0';

    // 解析组号
    PID_Params *target_pid = NULL;
    if (strstr(name, "_1") != NULL) {
        target_pid = pid_params_1;
    } else if (strstr(name, "_2") != NULL) {
        target_pid = pid_params_2;
    } else {
        printf("未知的 PID 组: %s\n", name);
        return;
    }

    // 去掉 "_1" 或 "_2" 后的参数名
    name[strlen(name) - 2] = '\0';

    // 解析数值部分
    if (strcmp(name, "SET") == 0) {
        // 解析整数值
        target_pid->set_flag = (uint8_t)strtol(colon_pos + 1, NULL, 10);
        printf("set %s: %d\n", input, target_pid->set_flag);
    } else {
        // 解析浮点数
        float value = strtof(colon_pos + 1, NULL);

        if (strcmp(name, "KP") == 0) {
            target_pid->KP = value;
        } else if (strcmp(name, "KI") == 0) {
            target_pid->KI = value;
        } else if (strcmp(name, "KD") == 0) {
            target_pid->KD = value;
        } else if (strcmp(name, "Iq") == 0) {
            target_pid->Iq = value;
        } else if (strcmp(name, "Id") == 0) {
            target_pid->Id = value;
        } else {
            printf("未知的参数: %s\n", name);
            return;
        }

    }
}
