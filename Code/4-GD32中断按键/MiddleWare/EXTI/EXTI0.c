#include "EXTI0.h"

void Exti0_Init(void){
    //引脚配置
    uint32_t RCU_GPIO = RCU_GPIOA; //选择GPIOA
    uint32_t GPIO_port = GPIOA; //选择GPIOA端口
    uint32_t GPIO_PIN = GPIO_PIN_0; //选择PA0引脚
    //外部中断配置
    uint32_t Souce_port = EXTI_SOURCE_GPIOA; //选择GPIOA作为外部中断源
    uint32_t Souce_pin = EXTI_SOURCE_PIN0; //选择PA0作为外部中断引脚
    uint32_t EXTI_LINE = EXTI_0; //选择外部中断线0
    uint32_t mode = EXTI_INTERRUPT; //设置为中断模式
    uint32_t trig_type = EXTI_TRIG_BOTH; //设置为双边沿触发
    uint32_t nvic_irq = EXTI0_IRQn; //选择EXTI0中断

    //配置GPIO
    rcu_periph_clock_enable(RCU_GPIO); //使能GPIOA时钟
    gpio_mode_set(GPIO_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN); //设置PA0为输入模式，无上拉下拉
    gpio_output_options_set(GPIO_port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN); //设置PA0为推挽输出，速度50MHz

    //外部中断初始化
    rcu_periph_clock_enable(RCU_SYSCFG); //使能SYSCFG时钟
    syscfg_exti_line_config(Souce_port, Souce_pin); //配置PA0为外部中断线0
    exti_init(EXTI_LINE, mode, trig_type); //配置外部中断0为上升沿触发

    //中断优先级
    nvic_irq_enable(nvic_irq, 2, 2); //使能EXTI0中断，优先级为0
    //使能外部中断
    exti_interrupt_enable(EXTI_LINE); //使能外部中断0
}

void EXTI0_IRQHandler(void) {
    if (exti_interrupt_flag_get(EXTI_0)) { //检查外部中断0是否触发
        exti_interrupt_flag_clear(EXTI_0); //清除外部中断0标志
        printf("hello \r\n"); //打印中断触发信息
    }
}