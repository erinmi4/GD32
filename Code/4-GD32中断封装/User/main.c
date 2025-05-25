
/**
 * @file main.c
 * @brief �������ļ�
 */
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "Usart0.h"
#include "Exti.h"

/**
 * @brief ����������ر���
 * 
 * ����������Ҫ:
 * 1. ����״̬��¼
 * 2. ������ʱ��ʱ
 */
/* PD0����״̬��ʱ���¼ */
FlagStatus pre = SET;
uint32_t pre_tick = 0;

/* PD1����״̬��ʱ���¼ */
FlagStatus pre1 = SET;
uint32_t pre1_tick = 0;

/* PD5����״̬��ʱ���¼ */
FlagStatus pre2 = SET;
uint32_t pre2_tick = 0;


/**
 * @brief ���ڽ��ջص�����
 * 
 * @param data ���յ������ݻ�����
 * @param len ���յ������ݳ���
 */
void Usart0_on_recv(uint8_t* data, uint32_t len) {
    printf("recv: %s\r\n", data);
}

/**
 * @brief �ⲿ�ж�0�����ص�����(PD0����)
 */
void Exti0_on_trig(void) {
    /* ��ȡ��ǰʱ�� */
    uint32_t current_tick = systick_tick_us();
    /* ��������(20ms) */
    if(current_tick - pre_tick > 20000) {
        pre_tick = current_tick;
        /* ��ȡ��ǰ����״̬ */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
        /* �����ͷ�(������) */
        if(SET == current && RESET == pre) {
            pre = current;
            printf("PD0 up \r\n");
        } 
        /* ��������(�½���) */
        else if(RESET == current && SET == pre) {
            pre = current;
            printf("PD0 down \r\n");
        }
    }
}

/**
 * @brief �ⲿ�ж�1�����ص�����(PD1����)
 */
void Exti1_on_trig(void) {
    /* ��ȡ��ǰʱ�� */
    uint32_t current_tick = systick_tick_us();
    /* ��������(20ms) */
    if(current_tick - pre1_tick > 20000) {
        pre1_tick = current_tick;
        /* ��ȡ��ǰ����״̬ */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_1);
        /* �����ͷ�(������) */
        if(SET == current && RESET == pre1) {
            pre1 = current;
            printf("PD1 up \r\n");
        } 
        /* ��������(�½���) */
        else if(RESET == current && SET == pre1) {
            pre1 = current;
            printf("PD1 down \r\n");
        }
    }
}

/**
 * @brief �ⲿ�ж�5�����ص�����(PD5����)
 */
void Exti5_on_trig(void) {
    /* ��ȡ��ǰʱ�� */
    uint32_t current_tick = systick_tick_us();
    /* ��������(20ms) */
    if(current_tick - pre2_tick > 20000) {
        pre2_tick = current_tick;
        /* ��ȡ��ǰ����״̬ */
        FlagStatus current = gpio_input_bit_get(GPIOD, GPIO_PIN_5);
        /* �����ͷ�(������) */
        if(SET == current && RESET == pre2) {
            pre2 = current;
            printf("PD5 up \r\n");
        } 
        /* ��������(�½���) */
        else if(RESET == current && SET == pre2) {
            pre2 = current;
            printf("PD5 down \r\n");
        }
    }
}



/**
 * @brief ������
 * 
 * @return int �����˳���
 */
int main(void)
{
    /* ��ʼ���δ�ʱ�� */
    systick_config();
    
    /* ��ʼ������ */
    Usart0_init();
    
    /* ��ʼ���ⲿ�ж� */
    Exti_init();

    /* ��ѭ�� */
    while(1) {
        /* ϵͳ���ж���������ѭ���������� */
    }
}
