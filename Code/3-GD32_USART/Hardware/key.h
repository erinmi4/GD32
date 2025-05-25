#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "gd32f4xx.h"
#include "systick.h"

// Key1为PC8
#define KEY1_RCU RCU_GPIOC
#define KEY1_PORT GPIOC
#define KEY1_PIN GPIO_PIN_8
// Key2为PC9
#define KEY2_RCU RCU_GPIOC
#define KEY2_PORT GPIOC
#define KEY2_PIN GPIO_PIN_9
//key3为PA0
#define KEY3_RCU RCU_GPIOA
#define KEY3_PORT GPIOA
#define KEY3_PIN GPIO_PIN_0
//key4为PC11
#define KEY4_RCU RCU_GPIOC
#define KEY4_PORT GPIOC
#define KEY4_PIN GPIO_PIN_11

void key_init(rcu_periph_enum KEY_RCU,uint32_t KEY_PORT,uint32_t KEY_PIN);
void key_start(void);
#endif