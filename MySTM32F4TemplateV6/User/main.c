
#include "main.h"
#include "systick.h"

//PG3
static void GPIO_config() {
    // rcu  ±÷”
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
    // gpio≈‰÷√
//    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
//    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_7);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
}



int main(void)
{
    systick_config();
    GPIO_config();

    while(1) {
        GPIO_SetBits(GPIOG, GPIO_Pin_3);
        delay_1ms(200);
        GPIO_ResetBits(GPIOG, GPIO_Pin_3);
        delay_1ms(200);
    }
}

