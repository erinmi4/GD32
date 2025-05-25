#ifndef __EXTI0_H__
#define __EXTI0_H__

#include "gd32f4xx.h"
#include <stdio.h>
#include "systick.h"

void Exti0_Hard_Init(void);//硬件中断
void Exti0_Soft_Init(void);//软件中断

#endif