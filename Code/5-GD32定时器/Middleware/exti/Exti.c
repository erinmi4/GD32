/**
 * @file Exti.c
 * @brief 外部中断实现文件
 */
#include "Exti.h"

/**
 * @brief 配置单个外部中断
 * @param exti_port_rcu   GPIO端口时钟
 * @param exti_port       GPIO端口
 * @param exti_pin        GPIO引脚
 * @param exti_src_port   外部中断源端口
 * @param exti_src_pin    外部中断源引脚
 * @param exti_pupd       引脚上拉下拉配置
 * @param exti            外部中断线
 * @param exti_irq        中断号
 * @param exti_trig       触发方式
 * @param exti_nvic_pre   抢占优先级
 * @param exti_nvic_sub   响应优先级
 */
static void Exti_config(uint32_t exti_port_rcu, uint32_t exti_port, uint32_t exti_pin,
                        uint32_t exti_src_port, uint32_t exti_src_pin,
                        uint32_t exti_pupd,
                        uint32_t exti, uint32_t exti_irq, uint8_t exti_trig,
                        uint8_t exti_nvic_pre, uint8_t exti_nvic_sub) {    /* GPIO初始化 */
    /* 使能GPIO端口时钟 */
    rcu_periph_clock_enable(exti_port_rcu);
    /* 配置GPIO模式 */
    gpio_mode_set(exti_port, GPIO_MODE_INPUT, exti_pupd, exti_pin);
    gpio_output_options_set(exti_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, exti_pin);

    /* 外部中断初始化 */
    /* 使能系统配置控制器时钟 */
    rcu_periph_clock_enable(RCU_SYSCFG);
    /* 配置外部中断源 */
    syscfg_exti_line_config(exti_src_port, exti_src_pin);
    /* 初始化外部中断 */
    exti_init(exti, EXTI_INTERRUPT, exti_trig);

    /* 配置中断优先级 */
    nvic_irq_enable(exti_irq, exti_nvic_pre, exti_nvic_sub);
    /* 使能外部中断 */
    exti_interrupt_enable(exti);
}

/**
 * @brief 初始化所有配置的外部中断
 */
void Exti_init(void) {
#if USE_EXTI_0
#if EXTI0_SW
    /* 软件中断模式不需要初始化GPIO */
#else
    /* 配置外部中断0 */
    Exti_config(EXTI0_PORT_RCU, EXTI0_PORT, GPIO_PIN_0,
                EXTI0_SRC_PORT, EXTI_SOURCE_PIN0,
                EXTI0_PUPD, EXTI_0, EXTI0_IRQn, EXTI0_TRIG, EXTI0_NVIC);
#endif    
#endif

#if USE_EXTI_1
    /* 配置外部中断1 */
    Exti_config(EXTI1_PORT_RCU, EXTI1_PORT, GPIO_PIN_1,
                EXTI1_SRC_PORT, EXTI_SOURCE_PIN1,
                EXTI1_PUPD, EXTI_1, EXTI1_IRQn, EXTI1_TRIG, EXTI1_NVIC);
#endif
    
#if USE_EXTI_5
    /* 配置外部中断5 */
    Exti_config(EXTI5_PORT_RCU, EXTI5_PORT, GPIO_PIN_5,
                EXTI5_SRC_PORT, EXTI_SOURCE_PIN5,
                EXTI5_PUPD, EXTI_5, EXTI5_9_IRQn, EXTI5_TRIG, EXTI5_NVIC);
#endif
}

#if USE_EXTI_0
/**
 * @brief 外部中断0中断处理函数
 */
void EXTI0_IRQHandler(void) {
    if(SET == exti_interrupt_flag_get(EXTI_0)) {
        Exti0_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_0);
}

#if EXTI0_SW
/**
 * @brief 触发外部中断0软件中断
 */
void Exti0_sw_trig(void) {
    exti_software_interrupt_enable(EXTI_0);
}
#endif
#endif

#if USE_EXTI_1
/**
 * @brief 外部中断1中断处理函数
 */
void EXTI1_IRQHandler(void) {
    if(SET == exti_interrupt_flag_get(EXTI_1)) {
        Exti1_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_1);
}
#endif

#if USE_EXTI_5 || USE_EXTI_6 || USE_EXTI_7 || USE_EXTI_8 || USE_EXTI_9
/**
 * @brief 外部中断5-9中断处理函数
 */
void EXTI5_9_IRQHandler(void) {
#if USE_EXTI_5
    if(SET == exti_interrupt_flag_get(EXTI_5)) {
        Exti5_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_5);
#endif
        
#if USE_EXTI_6
    if(SET == exti_interrupt_flag_get(EXTI_6)) {
        Exti6_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_6);
#endif
}
#endif