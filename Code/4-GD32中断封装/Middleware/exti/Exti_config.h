#ifndef __EXTI_CONFIG_H__
#define __EXTI_CONFIG_H__



#define USE_EXTI_0	1
#define USE_EXTI_1	1
#define USE_EXTI_2	0
#define USE_EXTI_3	0
#define USE_EXTI_4	0
#define USE_EXTI_5	1
#define USE_EXTI_6	0
#define USE_EXTI_7	0
#define USE_EXTI_8	0
#define USE_EXTI_9	0
#define USE_EXTI_10	0
#define USE_EXTI_11	0
#define USE_EXTI_12	0
#define USE_EXTI_13	0
#define USE_EXTI_14	0
#define USE_EXTI_15	0


/******************* EXTI 0 *************************/
#if USE_EXTI_0
#define EXTI0_SW				0
#if	EXTI0_SW == 0
#define EXTI0_PORT_RCU	RCU_GPIOD
#define EXTI0_PORT			GPIOD
#define EXTI0_SRC_PORT	EXTI_SOURCE_GPIOD
#define EXTI0_PUPD			GPIO_PUPD_PULLUP
#define EXTI0_TRIG			EXTI_TRIG_BOTH
#endif
#define EXTI0_NVIC			2, 2
#endif

/******************* EXTI 1 *************************/
#if USE_EXTI_1
#define EXTI1_PORT_RCU	RCU_GPIOD
#define EXTI1_PORT			GPIOD
#define EXTI1_SRC_PORT	EXTI_SOURCE_GPIOD
#define EXTI1_PUPD			GPIO_PUPD_PULLUP
#define EXTI1_TRIG			EXTI_TRIG_BOTH
#define EXTI1_NVIC			2, 2
#endif


/******************* EXTI 5 *************************/
#if USE_EXTI_5
#define EXTI5_PORT_RCU	RCU_GPIOD
#define EXTI5_PORT			GPIOD
#define EXTI5_SRC_PORT	EXTI_SOURCE_GPIOD
#define EXTI5_PUPD			GPIO_PUPD_PULLUP
#define EXTI5_TRIG			EXTI_TRIG_BOTH
#define EXTI5_NVIC			2, 2
#endif



#endif