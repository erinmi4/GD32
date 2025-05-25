
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "Usart0.h"

int main(void)
{
    systick_config();

    while(1) {
        //printf("Hello World! %d\n", cnt++); //打印数据
        //delay_1ms(1000); //延时1秒
    }
}
