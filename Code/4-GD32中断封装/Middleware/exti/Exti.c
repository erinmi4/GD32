/**
 * @file Exti.c
 * @brief �ⲿ�ж�ʵ���ļ�
 */
#include "Exti.h"

/**
 * @brief ���õ����ⲿ�ж�
 * @param exti_port_rcu   GPIO�˿�ʱ��
 * @param exti_port       GPIO�˿�
 * @param exti_pin        GPIO����
 * @param exti_src_port   �ⲿ�ж�Դ�˿�
 * @param exti_src_pin    �ⲿ�ж�Դ����
 * @param exti_pupd       ����������������
 * @param exti            �ⲿ�ж���
 * @param exti_irq        �жϺ�
 * @param exti_trig       ������ʽ
 * @param exti_nvic_pre   ��ռ���ȼ�
 * @param exti_nvic_sub   ��Ӧ���ȼ�
 */
static void Exti_config(uint32_t exti_port_rcu, uint32_t exti_port, uint32_t exti_pin,
                        uint32_t exti_src_port, uint32_t exti_src_pin,
                        uint32_t exti_pupd,
                        uint32_t exti, uint32_t exti_irq, uint8_t exti_trig,
                        uint8_t exti_nvic_pre, uint8_t exti_nvic_sub) {    /* GPIO��ʼ�� */
    /* ʹ��GPIO�˿�ʱ�� */
    rcu_periph_clock_enable(exti_port_rcu);
    /* ����GPIOģʽ */
    gpio_mode_set(exti_port, GPIO_MODE_INPUT, exti_pupd, exti_pin);
    gpio_output_options_set(exti_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, exti_pin);

    /* �ⲿ�жϳ�ʼ�� */
    /* ʹ��ϵͳ���ÿ�����ʱ�� */
    rcu_periph_clock_enable(RCU_SYSCFG);
    /* �����ⲿ�ж�Դ */
    syscfg_exti_line_config(exti_src_port, exti_src_pin);
    /* ��ʼ���ⲿ�ж� */
    exti_init(exti, EXTI_INTERRUPT, exti_trig);

    /* �����ж����ȼ� */
    nvic_irq_enable(exti_irq, exti_nvic_pre, exti_nvic_sub);
    /* ʹ���ⲿ�ж� */
    exti_interrupt_enable(exti);
}

/**
 * @brief ��ʼ���������õ��ⲿ�ж�
 */
void Exti_init(void) {
#if USE_EXTI_0
#if EXTI0_SW
    /* ����ж�ģʽ����Ҫ��ʼ��GPIO */
#else
    /* �����ⲿ�ж�0 */
    Exti_config(EXTI0_PORT_RCU, EXTI0_PORT, GPIO_PIN_0,
                EXTI0_SRC_PORT, EXTI_SOURCE_PIN0,
                EXTI0_PUPD, EXTI_0, EXTI0_IRQn, EXTI0_TRIG, EXTI0_NVIC);
#endif    
#endif

#if USE_EXTI_1
    /* �����ⲿ�ж�1 */
    Exti_config(EXTI1_PORT_RCU, EXTI1_PORT, GPIO_PIN_1,
                EXTI1_SRC_PORT, EXTI_SOURCE_PIN1,
                EXTI1_PUPD, EXTI_1, EXTI1_IRQn, EXTI1_TRIG, EXTI1_NVIC);
#endif
    
#if USE_EXTI_5
    /* �����ⲿ�ж�5 */
    Exti_config(EXTI5_PORT_RCU, EXTI5_PORT, GPIO_PIN_5,
                EXTI5_SRC_PORT, EXTI_SOURCE_PIN5,
                EXTI5_PUPD, EXTI_5, EXTI5_9_IRQn, EXTI5_TRIG, EXTI5_NVIC);
#endif
}

#if USE_EXTI_0
/**
 * @brief �ⲿ�ж�0�жϴ�����
 */
void EXTI0_IRQHandler(void) {
    if(SET == exti_interrupt_flag_get(EXTI_0)) {
        Exti0_on_trig();
    }
    exti_interrupt_flag_clear(EXTI_0);
}

#if EXTI0_SW
/**
 * @brief �����ⲿ�ж�0����ж�
 */
void Exti0_sw_trig(void) {
    exti_software_interrupt_enable(EXTI_0);
}
#endif
#endif

#if USE_EXTI_1
/**
 * @brief �ⲿ�ж�1�жϴ�����
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
 * @brief �ⲿ�ж�5-9�жϴ�����
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