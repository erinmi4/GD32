
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "Usart0.h"
#include "Exti.h"

// PD0按键，外部中断实现
//1. 状态记录
//2. 扫描延时
FlagStatus pre = SET;
uint32_t pre_tick = 0;
FlagStatus pre1 = SET;
uint32_t pre1_tick = 0;
FlagStatus pre2 = SET;
uint32_t pre2_tick = 0;


void Usart0_on_recv(uint8_t* data, uint32_t len) {

    printf("recv: %s\r\n", data);
}

// PD0

void Exti0_on_trig() {
    // 逻辑
    // 当前时间a
    uint32_t current_tick = systick_tick_us();
    if(current_tick - pre_tick > 20000) {
        pre_tick = current_tick;
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
        if(SET == current && RESET == pre) {
            pre = current;
            printf("PD0 up \r\n");
        } else if(RESET == current && SET == pre) {
            pre = current;
            printf("PD0 down \r\n");
        }
    }
}

void Exti1_on_trig() {
    // 逻辑
    // 当前时间a
    uint32_t current_tick = systick_tick_us();
    if(current_tick - pre1_tick > 20000) {
        pre1_tick = current_tick;
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_1);
        if(SET == current && RESET == pre1) {
            pre1 = current;
            printf("PD1 up \r\n");
        } else if(RESET == current && SET == pre1) {
            pre1 = current;
            printf("PD1 down \r\n");
        }
    }
}

void Exti5_on_trig() {
    // 逻辑
    // 当前时间a
    uint32_t current_tick = systick_tick_us();
    if(current_tick - pre2_tick > 20000) {
        pre2_tick = current_tick;
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_5);
        if(SET == current && RESET == pre2) {
            pre2 = current;
            printf("PD5 up \r\n");
        } else if(RESET == current && SET == pre2) {
            pre2 = current;
            printf("PD5 down \r\n");
        }
    }
}



int main(void)
{
    systick_config();
    Usart0_init();
    Exti_init();

    while(1) {

    }
}
