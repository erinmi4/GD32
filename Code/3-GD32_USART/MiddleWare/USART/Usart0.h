#ifndef __USART0_H__
#define __USART0_H__

#include "gd32f4xx.h"
#include <stdio.h>

void Usart0_init();

void Usart0_send_byte(uint8_t data);
void Usart0_send_data(uint8_t* data, uint32_t len);
void Usart0_send_string(uint8_t* data);

extern void Usart0_on_recv(uint8_t* data, uint32_t len);


#endif