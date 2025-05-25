#ifndef _BSP_BATTERAY_H
#define _BSP_BATTERAY_H

#include "gd32f4xx.h"
#include "systick.h"
//LED1为PC6
#define LED1_RCU RCU_GPIOC
#define LED1_PORT GPIOC
#define LED1_PIN GPIO_PIN_6
//LED2为PD1
#define LED2_RCU RCU_GPIOD
#define LED2_PORT GPIOD
#define LED2_PIN GPIO_PIN_1
//LED3为PC7
#define LED3_RCU RCU_GPIOC
#define LED3_PORT GPIOC
#define LED3_PIN GPIO_PIN_7
//LED4为PD0
#define LED4_RCU RCU_GPIOD
#define LED4_PORT GPIOD
#define LED4_PIN GPIO_PIN_0

//定义调用API
#define LED1_ON() gpio_bit_set(LED1_PORT, LED1_PIN)
#define LED1_OFF() gpio_bit_reset(LED1_PORT, LED1_PIN)

#define LED2_ON() gpio_bit_set(LED2_PORT, LED2_PIN)
#define LED2_OFF() gpio_bit_reset(LED2_PORT, LED2_PIN)

#define LED3_ON() gpio_bit_set(LED3_PORT, LED3_PIN)
#define LED3_OFF() gpio_bit_reset(LED3_PORT, LED3_PIN)

#define LED4_ON() gpio_bit_set(LED4_PORT, LED4_PIN)
#define LED4_OFF() gpio_bit_reset(LED4_PORT, LED4_PIN)

static void Led_Init(rcu_periph_enum periph, uint32_t port, uint32_t pin);

void battery_init(void);
void battery_start(uint16_t power);
void battery_stop(uint16_t power);
void battery_upstate(uint16_t power);
#endif