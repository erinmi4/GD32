/**
 * @file Usart0.c
 * @brief USART0����ͨ��ʵ���ļ�
 */
#include "Usart0.h"

/** ���ջ�������С */
#define RX_BUFFER_LEN       1024

/** ���ջ����� */
static uint8_t  rx_buffer[RX_BUFFER_LEN];
/** �������ݼ����� */
static uint32_t rx_cnt = 0;

/**
 * @brief ��ʼ��USART0����
 */
void Usart0_init(void) {
    /* USART0 TX�������� */
    uint32_t usart_tx_rcu = RCU_GPIOA;
    uint32_t usart_tx_port = GPIOA;
    uint32_t usart_tx_pin = GPIO_PIN_9;
    uint32_t usart_tx_af = GPIO_AF_7;
    
    /* USART0 RX�������� */
    uint32_t usart_rx_rcu = RCU_GPIOA;
    uint32_t usart_rx_port = GPIOA;
    uint32_t usart_rx_pin = GPIO_PIN_10;
    uint32_t usart_rx_af = GPIO_AF_7;
    
    /* USART0���� */
    uint32_t usart_rcu = RCU_USART0;
    uint32_t usart = USART0;
    uint32_t usart_irq = USART0_IRQn;
    
    /* ���ڲ������� */
    uint32_t usart_param_baudrate = 115200;    /* GPIO��AFIO���� */
    /* TX���ų�ʼ��: PA9 */
    rcu_periph_clock_enable(usart_tx_rcu);
    gpio_mode_set(usart_tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_tx_pin);
    gpio_output_options_set(usart_tx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_tx_pin);
    gpio_af_set(usart_tx_port, usart_tx_af, usart_tx_pin);
    
    /* RX���ų�ʼ��: PA10 */
    rcu_periph_clock_enable(usart_rx_rcu);
    gpio_mode_set(usart_rx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_rx_pin);
    gpio_output_options_set(usart_rx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_rx_pin);
    gpio_af_set(usart_rx_port, usart_rx_af, usart_rx_pin);

    /* ���ڿ�������ʼ�� */
    /* ʹ�ܴ���ʱ�� */
    rcu_periph_clock_enable(usart_rcu);
    usart_deinit(usart);
    
    /* ���ڲ������� */
    usart_baudrate_set(usart, usart_param_baudrate);  /* ������ */
    usart_parity_config(usart, USART_PM_NONE);        /* ��У�� */
    usart_word_length_set(usart, USART_WL_8BIT);      /* 8λ����λ */
    usart_stop_bit_set(usart, USART_STB_1BIT);        /* 1λֹͣλ */
    usart_data_first_config(usart, USART_MSBF_LSB);   /* LSB�ȷ��� */

    /* ʹ�ܷ��͹��� */
    usart_transmit_config(usart, USART_TRANSMIT_ENABLE);
    
    /* ʹ�ܽ��չ��� */
    usart_receive_config(usart, USART_RECEIVE_ENABLE);
    
    /* �����ж����� */
    nvic_irq_enable(usart_irq, 2, 2);
    usart_interrupt_enable(usart, USART_INT_RBNE);    /* ���ջ�������Ϊ���ж� */
    usart_interrupt_enable(usart, USART_INT_IDLE);    /* ���տ����ж� */

    /* ʹ�ܴ��� */
    usart_enable(usart);
}

/**
 * @brief ���͵����ֽ�����
 * 
 * @param data Ҫ���͵��ֽ�����
 */
void Usart0_send_byte(uint8_t data) {
    /* ͨ��USART���� */
    usart_data_transmit(USART0, data);
    /* �ȴ�������� */
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

/**
 * @brief ����ָ�����ȵ�����
 * 
 * @param data ���ݻ�����ָ��
 * @param len ���ݳ���
 */
void Usart0_send_data(uint8_t* data, uint32_t len) {
    while(data && *data && len) {
        Usart0_send_byte((uint8_t)(*data));
        data++;
        len--;
    }
}

/**
 * @brief ������'\0'��β���ַ���
 * 
 * @param data �ַ���ָ��
 */
void Usart0_send_string(uint8_t* data) {
    /* 1.dataָ�벻Ϊ��  2.���͵����ݲ���\0������� */
    while(data && *data) {
        Usart0_send_byte((uint8_t)(*data));
        data++;
    }
}

/**
 * @brief �ض���printf����������
 */
int fputc(int ch, FILE *f) {
    Usart0_send_byte((uint8_t)(ch));
    return ch;
}

/**
 * @brief USART0�жϴ�����
 */
void USART0_IRQHandler(void) {
    /* �жϺ���ֻ��1�����������жϵı�ǿ����ж�� */
    
    /* ���ջ������ǿ��жϴ��� */
    if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
        /* ����жϱ�־ */
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        
        /* ��ȡ���յ������� */
        uint8_t data = usart_data_receive(USART0);
        rx_buffer[rx_cnt++] = data;
        
        /* ȷ����������������С */
        if(rx_cnt >= RX_BUFFER_LEN - 1) {
            rx_cnt = RX_BUFFER_LEN - 1;
        }
    }
    
    /* ���տ����жϴ��� */
    if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
        /* ��ȡ���ݼĴ������IDLE��־ */
        usart_data_receive(USART0);
        
        /* ����ַ��������� */
        rx_buffer[rx_cnt] = '\0';
        
        /* ���ý��ջص������������� */
        if(rx_cnt > 0) {
            Usart0_on_recv(rx_buffer, rx_cnt);
        }
        
        /* ��ս��ռ����� */
        rx_cnt = 0;
    }
}