#include "timer.h"

void timer_config(void){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5); //����PA5Ϊ���ù���
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //����PA5Ϊ�������������ٶ�
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5); //����PA5ΪTIMER1_CH0�ĸ��ù���
    //ʱ��
    rcu_periph_clock_enable(RCU_TIMER1);
    //��ʼ����ʱ��
    timer_deinit(TIMER1);
    //��Ƶ4������Ϊʹ�õ���systemclock����Ҫ�ֲ����
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); 
    timer_parameter_struct timer_initpara;//����Ĭ��ֵ
    //��ʼ����ʱ������
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 10000 - 1; //Ԥ��Ƶֵ
    timer_initpara.period    = SystemCoreClock / 10000 - 1; //��������
    timer_init(TIMER1, &timer_initpara); //��ʱ��5��ʼ��
    //��ʱ��ͨ������
    timer_oc_parameter_struct timer_oc_initpara;
    timer_channel_output_struct_para_init(&timer_oc_initpara); //��ʼ��ͨ������
    timer_oc_initpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
    // timer_oc_initpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    // timer_oc_initpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;

    // timer_oc_initpara->outputnstate = TIMER_CCXN_DISABLE;
    // timer_oc_initpara->ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    // timer_oc_initpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_oc_initpara); //����TIMER1_CH0ͨ��
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0); //����TIMER1_CH0ͨ��ΪPWM0ģʽ
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0); //����TIMER1_CH0ͨ����PWM����ֵΪ5000
    //���ö�ʱ�ж�
    nvic_irq_enable(TIMER1_IRQn, 2, 2); //ʹ��TIMER1�ж�
    timer_interrupt_enable(TIMER1, TIMER_INT_UP); //ʹ��TIMER1�����ж�
    //�򿪼�ʱ��
    timer_enable(TIMER1);
}

void TIMER1_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        //��������Ӷ�ʱ���жϴ������
        printf("TIMER1 interrupt triggered!\n");
        // �Ƴ����ö�ʱ���Ĵ��룬�Ա���PWM�������
    }
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP); //����жϱ�־
}

/* 
    * @brief ����PWMռ�ձ�
    * @param duty: PWMռ�ձȣ���Χ0-100
    * 
    * ��������ֵ��ʽ��pulse_value = (SystemCoreClock / 10000) * duty - 1
    * ����SystemCoreClock��ϵͳʱ��Ƶ�ʣ�10000��PWMƵ�ʣ�10kHz��
*/
void pwm_update(float duty){
    if(duty > 100.0f) {
        duty = 100.0f; //����ռ�ձ����ֵΪ100%
    }
    else if(duty < 0.0f) {
        duty = 0.0f; //����ռ�ձ���СֵΪ0%
    }
    uint32_t period = SystemCoreClock / 10000 - 1; //��ȡ��������
    uint32_t pulse = (uint32_t)(period * duty / 100); //ֱ�Ӹ���ռ�ձȼ�������ֵ
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, pulse); //����TIMER1_CH0ͨ����PWM����ֵ
}
