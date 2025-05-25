#include "key.h"

void key_init(rcu_periph_enum KEY_RCU,uint32_t KEY_PORT,uint32_t KEY_PIN) {
    rcu_periph_clock_enable(KEY_RCU);
    gpio_mode_set(KEY_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_PIN);
}

void key_start(void) {
    key_init(KEY1_RCU, KEY1_PORT, KEY1_PIN);
    key_init(KEY2_RCU, KEY2_PORT, KEY2_PIN);
    key_init(KEY3_RCU, KEY3_PORT, KEY3_PIN);
    //key_init(KEY4_RCU, KEY4_PORT, KEY4_PIN);
}

