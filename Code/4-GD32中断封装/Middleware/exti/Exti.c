#include "Exti.h"


static void Exti_config(uint32_t exti_port_rcu,uint32_t exti_port,uint32_t exti_pin,
                        uint32_t exti_src_port,uint32_t exti_src_pin,
                        uint32_t exti_pupd,
                        uint32_t exti,uint32_t exti_irq,uint8_t exti_trig,
                        uint8_t exti_nvic_pre,uint8_t exti_nvic_sub) {
    //GPIO初始化
    //rcu
    rcu_periph_clock_enable(exti_port_rcu);
    //gpio
    gpio_mode_set(exti_port, GPIO_MODE_INPUT, exti_pupd, exti_pin);
    gpio_output_options_set(exti_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, exti_pin);

    //外部中断初始化
    //rcu
    rcu_periph_clock_enable(RCU_SYSCFG);
    //exti
    syscfg_exti_line_config(exti_src_port, exti_src_pin);
    exti_init(exti, EXTI_INTERRUPT, exti_trig);// 触发条件：both

    // 中断优先级
    nvic_irq_enable(exti_irq, exti_nvic_pre, exti_nvic_sub);
    // 外部中断函数
    exti_interrupt_enable(exti);
}

void Exti_init() {
#if USE_EXTI_0
#if EXTI0_SW
	
#else
	Exti_config(EXTI0_PORT_RCU, EXTI0_PORT, GPIO_PIN_0,
                EXTI0_SRC_PORT, EXTI_SOURCE_PIN0,
                EXTI0_PUPD, EXTI_0, EXTI0_IRQn, EXTI0_TRIG, EXTI0_NVIC);
#endif
    
#endif

#if USE_EXTI_1
    Exti_config(EXTI1_PORT_RCU, EXTI1_PORT, GPIO_PIN_1,
                EXTI1_SRC_PORT, EXTI_SOURCE_PIN1,
                EXTI1_PUPD, EXTI_1, EXTI1_IRQn, EXTI1_TRIG, EXTI1_NVIC);
#endif
	
#if USE_EXTI_5
    Exti_config(EXTI5_PORT_RCU, EXTI5_PORT, GPIO_PIN_5,
                EXTI5_SRC_PORT, EXTI_SOURCE_PIN5,
                EXTI5_PUPD, EXTI_5, EXTI5_9_IRQn, EXTI5_TRIG, EXTI5_NVIC);
#endif
}

#if USE_EXTI_0
void EXTI0_IRQHandler() {
    if(SET == exti_interrupt_flag_get(EXTI_0)) {
        Exti0_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_0);
}

#if EXTI0_SW
void Exti0_sw_trig() {
		exti_software_interrupt_enable(EXTI_0);
}
#endif
#endif

#if USE_EXTI_1
void EXTI1_IRQHandler() {
    if(SET == exti_interrupt_flag_get(EXTI_1)) {
        Exti1_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_1);
}
#endif

#if USE_EXTI_5 || USE_EXTI_6 || USE_EXTI_7 || USE_EXTI_8 || USE_EXTI_9
void EXTI5_9_IRQHandler() {
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