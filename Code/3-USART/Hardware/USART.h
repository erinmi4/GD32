#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#define RX_BUFFER_SIZE 1024 //接收缓冲区大小
extern uint8_t rx_buffer[RX_BUFFER_SIZE]; //接收缓冲区
extern uint32_t rx_cnt; //接收计数

void usart_config(void);
void usart_send_data(uint8_t data);
void usart_send_string(char *str);
int fputc(int ch, FILE *f);
#endif