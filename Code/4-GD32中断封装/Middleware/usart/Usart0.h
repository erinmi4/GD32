/**
 * @file Usart0.h
 * @brief USART0串口通信接口头文件
 */
#ifndef __USART0_H__
#define __USART0_H__

#include "gd32f4xx.h"
#include <stdio.h>

/**
 * @brief 初始化USART0串口
 */
void Usart0_init(void);

/**
 * @brief 发送单个字节数据
 * 
 * @param data 要发送的字节数据
 */
void Usart0_send_byte(uint8_t data);

/**
 * @brief 发送指定长度的数据
 * 
 * @param data 数据缓冲区指针
 * @param len 数据长度
 */
void Usart0_send_data(uint8_t* data, uint32_t len);

/**
 * @brief 发送以'\0'结尾的字符串
 * 
 * @param data 字符串指针
 */
void Usart0_send_string(uint8_t* data);

/**
 * @brief 串口接收回调函数(需在应用程序中实现)
 * 
 * @param data 接收到的数据缓冲区
 * @param len 接收到的数据长度
 */
extern void Usart0_on_recv(uint8_t* data, uint32_t len);

#endif /* __USART0_H__ */