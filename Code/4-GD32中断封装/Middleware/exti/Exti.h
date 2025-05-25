#ifndef __EXTI_H__
#define __EXTI_H__

#include "gd32f4xx.h"
#include "systick.h"
#include "Exti_config.h"

void Exti_init();

#if USE_EXTI_0
extern void Exti0_on_trig();

#if EXTI0_SW
void Exti0_sw_trig();
#endif
#endif

#if USE_EXTI_1
extern void Exti1_on_trig();
#endif

#if USE_EXTI_5
extern void Exti5_on_trig();
#endif

#endif