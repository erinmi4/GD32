
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "USART.h"

int main(void)
{
    systick_config();
    usart_config(); //配置串口
    uint16_t cnt = 0;
    while(1) {
        printf("Hello World! %d\n", cnt++); //打印数据
        delay_1ms(1000); //延时1秒
    }
}
