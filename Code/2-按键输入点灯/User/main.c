
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

// PA0: 按键
// 1. 原理图引脚
// 2. 原理图输入配置，默认值（1：外部默认 2：未知的）

// PD7
static void GPIO_config() {
	// rcu 时钟
	rcu_periph_clock_enable(RCU_GPIOD);
	// gpio配置
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_7);
	
		// gpio配置
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_1);
	
	
	// 初始化
	rcu_periph_clock_enable(RCU_GPIOA);
	//gpio out in
	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0); // 浮空输入，因为未知输入状态
	
		// 初始化
	rcu_periph_clock_enable(RCU_GPIOD);
	//gpio out in
	gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0); // 浮空输入，因为未知输入状态
	
	// u16: 1000 0000 0000 0001
}

int main(void)
{
    systick_config();
		GPIO_config();
		FlagStatus pre = RESET;
    while(1) {
//				gpio_bit_set(GPIOD, GPIO_PIN_7);
//				delay_1ms(1000);
//				gpio_bit_reset(GPIOD, GPIO_PIN_7);
//				delay_1ms(1000);
				FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
				
				if(SET == current && RESET == pre) {
					// 按下
					pre = current;
					gpio_bit_set(GPIOD, GPIO_PIN_7);
					gpio_bit_set(GPIOD, GPIO_PIN_1);
				} else if(RESET == current && SET == pre) {
					// 抬起
					pre = current;
					gpio_bit_reset(GPIOD, GPIO_PIN_7);
					gpio_bit_reset(GPIOD, GPIO_PIN_1);
				}
			
				delay_1ms(20);
    }
}
