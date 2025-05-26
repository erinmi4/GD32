/**
 * @file Usart0.h
 * @brief USART0串口通信接口头文件
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "gd32f4xx.h"
#include <stdio.h>

/*
* @brief 定时器配置函数
* @param None
*/
void timer_config(void);
/* 
* @brief PWM更新函数
* @param duty: PWM占空比，范围0.0到1.0
*/
void pwm_update(float duty);

#endif // __TIMER_H__