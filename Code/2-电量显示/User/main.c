
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "batteray.h"
#include "key.h"

uint16_t power = 1;// power 0 -4
int main(void)
{
    systick_config();
	battery_init();
	key_start();
	power = 2; // 初始电量

	FlagStatus pre1 = SET;// 默认值
	FlagStatus pre2 = SET;// 默认值
	FlagStatus pre3 = SET;// 默认值
	while (1)
	{
				delay_1ms(20);
			
				FlagStatus current1 = gpio_input_bit_get(KEY1_PORT, KEY1_PIN);
				
				if(SET == current1 && RESET == pre1) {
					// 抬起, 开始充电
					pre1 = current1;
					// TODO: 显示现在的电量，x并且流水灯????
					battery_start(power);
				} else if(RESET == current1 && SET == pre1) {
					// 按下
					pre1 = current1;
				}
				
				FlagStatus current2 = gpio_input_bit_get(KEY2_PORT, KEY2_PIN);
				
				if(SET == current2 && RESET == pre2) {
					// 抬起, 结束充电，TODO:
					pre2 = current2;
					battery_stop(power);
					power = 0; // 结束充电，电量清零
				} else if(RESET == current2 && SET == pre2) {
					// 按下
					pre2 = current2;
				}
				
				FlagStatus current3 = gpio_input_bit_get(KEY3_PORT, KEY3_PIN);
				
				if(SET == current3 && RESET == pre3) {
					// 抬起, 模拟电量增加
					pre3 = current3;
					
					power++;
					if(power > 4) power = 0;
					battery_upstate(power);
				} else if(RESET == current3 && SET == pre3) {
					// 按下
					pre3 = current3;
				}

    }
}
