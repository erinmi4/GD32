#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

void usart_config(void);
void usart_send_data(uint8_t data);
void usart_send_string(char *str);
int fputc(int ch, FILE *f);
#endif