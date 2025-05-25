
/**
 * @file main.c
 * @brief 主程序文件
 */
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "Usart0.h"
#include "Exti.h"

/**
 * @brief 按键防抖相关变量
 * 
 * 按键处理需要:
 * 1. 按键状态记录
 * 2. 消抖延时计时
 */
/* PD0按键状态和时间记录 */
FlagStatus pre = SET;
uint32_t pre_tick = 0;

/* PD1按键状态和时间记录 */
FlagStatus pre1 = SET;
uint32_t pre1_tick = 0;

/* PD5按键状态和时间记录 */
FlagStatus pre2 = SET;
uint32_t pre2_tick = 0;


/**
 * @brief 串口接收回调函数
 * 
 * @param data 接收到的数据缓冲区
 * @param len 接收到的数据长度
 */
void Usart0_on_recv(uint8_t* data, uint32_t len) {
    printf("recv: %s\r\n", data);
}

/**
 * @brief 外部中断0触发回调函数(PD0按键)
 */
void Exti0_on_trig(void) {
    /* 获取当前时间 */
    uint32_t current_tick = systick_tick_us();
    /* 按键消抖(20ms) */
    if(current_tick - pre_tick > 20000) {
        pre_tick = current_tick;
        /* 获取当前按键状态 */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
        /* 按键释放(上升沿) */
        if(SET == current && RESET == pre) {
            pre = current;
            printf("PD0 up \r\n");
        } 
        /* 按键按下(下降沿) */
        else if(RESET == current && SET == pre) {
            pre = current;
            printf("PD0 down \r\n");
        }
    }
}

/**
 * @brief 外部中断1触发回调函数(PD1按键)
 */
void Exti1_on_trig(void) {
    /* 获取当前时间 */
    uint32_t current_tick = systick_tick_us();
    /* 按键消抖(20ms) */
    if(current_tick - pre1_tick > 20000) {
        pre1_tick = current_tick;
        /* 获取当前按键状态 */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_1);
        /* 按键释放(上升沿) */
        if(SET == current && RESET == pre1) {
            pre1 = current;
            printf("PD1 up \r\n");
        } 
        /* 按键按下(下降沿) */
        else if(RESET == current && SET == pre1) {
            pre1 = current;
            printf("PD1 down \r\n");
        }
    }
}

/**
 * @brief 外部中断5触发回调函数(PD5按键)
 */
void Exti5_on_trig(void) {
    /* 获取当前时间 */
    uint32_t current_tick = systick_tick_us();
    /* 按键消抖(20ms) */
    if(current_tick - pre2_tick > 20000) {
        pre2_tick = current_tick;
        /* 获取当前按键状态 */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_5);
        /* 按键释放(上升沿) */
        if(SET == current && RESET == pre2) {
            pre2 = current;
            printf("PD5 up \r\n");
        } 
        /* 按键按下(下降沿) */
        else if(RESET == current && SET == pre2) {
            pre2 = current;
            printf("PD5 down \r\n");
        }
    }
}



/**
 * @brief 主函数
 * 
 * @return int 程序退出码
 */
int main(void)
{
    /* 初始化滴答定时器 */
    systick_config();
    
    /* 初始化串口 */
    Usart0_init();
    
    /* 初始化外部中断 */
    Exti_init();

    /* 主循环 */
    while(1) {
        /* 系统由中断驱动，主循环暂无任务 */
    }
}
