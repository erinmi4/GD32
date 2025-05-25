#include"batteray.h"


void Led_Init(rcu_periph_enum periph,uint32_t port,uint32_t pin){
    rcu_periph_clock_enable(periph);
	gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
	gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pin);
}

void battery_init(void) {
    Led_Init(LED1_RCU, LED1_PORT, LED1_PIN);
    Led_Init(LED2_RCU, LED2_PORT, LED2_PIN);
    Led_Init(LED3_RCU, LED3_PORT, LED3_PIN);
    Led_Init(LED4_RCU, LED4_PORT, LED4_PIN);
}

//开启，显示当前电量
void battery_start(uint16_t power){
    switch (power)
    {
    case 1:
        LED1_ON();
        LED2_OFF();
        LED3_OFF();
        LED4_OFF();
        break;
    case 2:
        LED1_ON();
        LED2_ON();
        LED3_OFF();
        LED4_OFF();
        break;
    case 3:
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_OFF();
        break;
    case 4:
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_ON();
        break;
    default:
        break;
    }
}

//关闭，当前电量闪烁3下
void battery_stop(uint16_t power){
    switch (power)
    {
    case 1:
        for (int i = 0; i < 3; i++)
        {
            LED1_ON();
            delay_1ms(500);
            LED1_OFF();
            delay_1ms(500);
        }
        break;
    case 2:
        for (int i = 0; i < 3; i++)
        {
            LED1_ON();
            LED2_ON();
            delay_1ms(500);
            LED1_OFF();
            LED2_OFF();
            delay_1ms(500);
        }
        break;
    case 3:
        for (int i = 0; i < 3; i++)
        {
            LED1_ON();
            LED2_ON();
            LED3_ON();
            delay_1ms(500);
            LED1_OFF();
            LED2_OFF();
            LED3_OFF();
            delay_1ms(500);
        }
        break;
    case 4:
        for (int i = 0; i < 3; i++)
        {
            LED1_ON();
            LED2_ON();
            LED3_ON();
            LED4_ON();
            delay_1ms(500);
            LED1_OFF();
            LED2_OFF();
            LED3_OFF();
            LED4_OFF();
            delay_1ms(500);
        }
        break;
    default:
        break;
    }

}

//更新电量
void battery_upstate(uint16_t power){
    switch (power)
    {
    case 1:
        LED1_ON();
        LED2_OFF();
        LED3_OFF();
        LED4_OFF();
        break;
    case 2:
        LED1_ON();
        LED2_ON();
        LED3_OFF();
        LED4_OFF();
        break;
    case 3:
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_OFF();
        break;
    case 4:
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_ON();
        break;
    default:
        break;
    }
}