#include "timer.h"

void timer_config(void){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5); //配置PA5为复用功能
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //配置PA5为推挽输出，最大速度
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5); //设置PA5为TIMER1_CH0的复用功能
    //时钟
    rcu_periph_clock_enable(RCU_TIMER1);
    //初始化计时器
    timer_deinit(TIMER1);
    //升频4倍，因为使用的是systemclock所以要弥补差距
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); 
    timer_parameter_struct timer_initpara;//配置默认值
    //初始化计时器参数
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 10000 - 1; //预分频值
    timer_initpara.period    = SystemCoreClock / 10000 - 1; //计数周期
    timer_init(TIMER1, &timer_initpara); //计时器5初始化
    //定时器通道配置
    timer_oc_parameter_struct timer_oc_initpara;
    timer_channel_output_struct_para_init(&timer_oc_initpara); //初始化通道参数
    timer_oc_initpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
    // timer_oc_initpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    // timer_oc_initpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;

    // timer_oc_initpara->outputnstate = TIMER_CCXN_DISABLE;
    // timer_oc_initpara->ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    // timer_oc_initpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_oc_initpara); //配置TIMER1_CH0通道
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0); //配置TIMER1_CH0通道为PWM0模式
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0); //设置TIMER1_CH0通道的PWM脉冲值为5000
    //配置定时中断
    nvic_irq_enable(TIMER1_IRQn, 2, 2); //使能TIMER1中断
    timer_interrupt_enable(TIMER1, TIMER_INT_UP); //使能TIMER1更新中断
    //打开计时器
    timer_enable(TIMER1);
}

void TIMER1_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        //在这里添加定时器中断处理代码
        printf("TIMER1 interrupt triggered!\n");
        // 移除禁用定时器的代码，以保持PWM输出连续
    }
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP); //清除中断标志
}

/* 
    * @brief 更新PWM占空比
    * @param duty: PWM占空比，范围0-100
    * 
    * 计算脉冲值公式：pulse_value = (SystemCoreClock / 10000) * duty - 1
    * 其中SystemCoreClock是系统时钟频率，10000是PWM频率（10kHz）
*/
void pwm_update(float duty){
    if(duty > 100.0f) {
        duty = 100.0f; //限制占空比最大值为100%
    }
    else if(duty < 0.0f) {
        duty = 0.0f; //限制占空比最小值为0%
    }
    uint32_t period = SystemCoreClock / 10000 - 1; //获取计数周期
    uint32_t pulse = (uint32_t)(period * duty / 100); //直接根据占空比计算脉冲值
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, pulse); //更新TIMER1_CH0通道的PWM脉冲值
}
