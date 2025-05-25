#include "EXTI0.h"

void Exti0_Soft_Init(void){
    //外部中断配置
	uint32_t exti = EXTI_0;
	uint32_t exti_irq = EXTI0_IRQn;

    //外部中断初始化
    //rcu
    rcu_periph_clock_enable(RCU_SYSCFG);
    //exti
    exti_init(exti, EXTI_INTERRUPT, EXTI_TRIG_NONE);

    // 中断优先级
    nvic_irq_enable(exti_irq, 2, 2);
    // 外部中断函数
    exti_interrupt_enable(exti);
}
void Exti0_Hard_Init(void){
    //引脚配置
    uint32_t RCU_GPIO = RCU_GPIOA; //选择GPIOA
    uint32_t GPIO_port = GPIOA; //选择GPIOA端口
    uint32_t GPIO_PIN = GPIO_PIN_0; //选择PA0引脚
    uint32_t exti_pupd = GPIO_PUPD_NONE; //无上拉下拉
    //外部中断配置
    uint32_t Souce_port = EXTI_SOURCE_GPIOA; //选择GPIOA作为外部中断源
    uint32_t Souce_pin = EXTI_SOURCE_PIN0; //选择PA0作为外部中断引脚
    uint32_t EXTI_LINE = EXTI_0; //选择外部中断线0
    uint32_t mode = EXTI_INTERRUPT; //设置为中断模式
    uint8_t trig_type = EXTI_TRIG_BOTH; //设置为双边沿触发
    uint32_t nvic_irq = EXTI0_IRQn; //选择EXTI0中断

    //配置GPIO
    rcu_periph_clock_enable(RCU_GPIO); //使能GPIOA时钟
    gpio_mode_set(GPIO_port, GPIO_MODE_INPUT, exti_pupd, GPIO_PIN); //设置PA0为输入模式，无上拉下拉
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

FlagStatus pre = RESET; //定义一个标志位，用于记录上一次中断状态
uint32_t pre_tick = 0; //定义一个变量，用于记录上一次中断时间

void EXTI0_IRQHandler(void) {
    if (exti_interrupt_flag_get(EXTI_0)) { //检查外部中断0是否触发
        //获取当前时间
        uint32_t current_tick = systick_tick_us(); 
        //如果距离上次中断时间超过20ms
        if (current_tick - pre_tick > 20 * 0000) { 
            pre_tick = current_tick; //更新上一次中断时间
            FlagStatus current = gpio_input_bit_get(GPIOA, GPIO_PIN_0); //获取PA0的当前状态
        //在时间差大于20ms的情况下，判断当前状态与上一次状态是否相同
        if (current != pre) { //如果当前状态与上一次状态不同
            pre = current; //更新上一次状态
            if (current == SET) {
                printf("PA0 is pressed!\n"); //打印按下信息
            } else {
                printf("PA0 is released!\n"); //打印释放信息
            }
        }
        } 
        exti_interrupt_flag_clear(EXTI_0); //清除外部中断标志位
    }
}