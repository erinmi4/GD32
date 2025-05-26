/**
 * @file Exti_config.h
 * @brief 外部中断配置文件
 */
#ifndef __EXTI_CONFIG_H__
#define __EXTI_CONFIG_H__

/**
 * @brief 外部中断使能配置
 * @note 1: 使能, 0: 禁用
 */
#define USE_EXTI_0    0  /* 外部中断0使能 */
#define USE_EXTI_1    0  /* 外部中断1使能 */
#define USE_EXTI_2    0  /* 外部中断2禁用 */
#define USE_EXTI_3    0  /* 外部中断3禁用 */
#define USE_EXTI_4    0  /* 外部中断4禁用 */
#define USE_EXTI_5    0  /* 外部中断5使能 */
#define USE_EXTI_6    0  /* 外部中断6禁用 */
#define USE_EXTI_7    0  /* 外部中断7禁用 */
#define USE_EXTI_8    0  /* 外部中断8禁用 */
#define USE_EXTI_9    0  /* 外部中断9禁用 */
#define USE_EXTI_10   0  /* 外部中断10禁用 */
#define USE_EXTI_11   0  /* 外部中断11禁用 */
#define USE_EXTI_12   0  /* 外部中断12禁用 */
#define USE_EXTI_13   0  /* 外部中断13禁用 */
#define USE_EXTI_14   0  /* 外部中断14禁用 */
#define USE_EXTI_15   0  /* 外部中断15禁用 */


/**
 * @brief EXTI0 配置参数
 */
#if USE_EXTI_0
#define EXTI0_SW            0  /* 0:硬件触发, 1:软件触发 */
#if EXTI0_SW == 0
#define EXTI0_PORT_RCU      RCU_GPIOD     /* GPIO端口时钟 */
#define EXTI0_PORT          GPIOD         /* GPIO端口 */
#define EXTI0_SRC_PORT      EXTI_SOURCE_GPIOD  /* 外部中断源端口 */
#define EXTI0_PUPD          GPIO_PUPD_PULLUP   /* 引脚上拉下拉配置 */
#define EXTI0_TRIG          EXTI_TRIG_BOTH     /* 触发方式 */
#endif
#define EXTI0_NVIC          2, 2          /* 中断优先级配置 */
#endif

/**
 * @brief EXTI1 配置参数
 */
#if USE_EXTI_1
#define EXTI1_PORT_RCU      RCU_GPIOD     /* GPIO端口时钟 */
#define EXTI1_PORT          GPIOD         /* GPIO端口 */
#define EXTI1_SRC_PORT      EXTI_SOURCE_GPIOD  /* 外部中断源端口 */
#define EXTI1_PUPD          GPIO_PUPD_PULLUP   /* 引脚上拉下拉配置 */
#define EXTI1_TRIG          EXTI_TRIG_BOTH     /* 触发方式 */
#define EXTI1_NVIC          2, 2          /* 中断优先级配置 */
#endif

/**
 * @brief EXTI5 配置参数
 */
#if USE_EXTI_5
#define EXTI5_PORT_RCU      RCU_GPIOD     /* GPIO端口时钟 */
#define EXTI5_PORT          GPIOD         /* GPIO端口 */
#define EXTI5_SRC_PORT      EXTI_SOURCE_GPIOD  /* 外部中断源端口 */
#define EXTI5_PUPD          GPIO_PUPD_PULLUP   /* 引脚上拉下拉配置 */
#define EXTI5_TRIG          EXTI_TRIG_BOTH     /* 触发方式 */
#define EXTI5_NVIC          2, 2          /* 中断优先级配置 */
#endif

#endif /* __EXTI_CONFIG_H__ */