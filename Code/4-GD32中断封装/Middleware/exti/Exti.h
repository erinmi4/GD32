/**
 * @file Exti.h
 * @brief 外部中断接口头文件
 */
#ifndef __EXTI_H__
#define __EXTI_H__

#include "gd32f4xx.h"
#include "systick.h"
#include "Exti_config.h"

/**
 * @brief 初始化所有配置的外部中断
 */
void Exti_init(void);

#if USE_EXTI_0
/**
 * @brief 外部中断0触发回调函数(需在应用程序中实现)
 */
extern void Exti0_on_trig(void);

#if EXTI0_SW
/**
 * @brief 触发外部中断0软件中断
 */
void Exti0_sw_trig(void);
#endif
#endif

#if USE_EXTI_1
/**
 * @brief 外部中断1触发回调函数(需在应用程序中实现)
 */
extern void Exti1_on_trig(void);
#endif

#if USE_EXTI_5
/**
 * @brief 外部中断5触发回调函数(需在应用程序中实现)
 */
extern void Exti5_on_trig(void);
#endif

#endif /* __EXTI_H__ */