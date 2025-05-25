#include "Usart0.h"

/* 接收缓冲区定义 */
#define RX_BUFFER_LEN    1024
static uint8_t  rx_buffer[RX_BUFFER_LEN];
static uint32_t rx_cnt = 0;

/**
 * @brief 初始化USART0
 * @note  配置GPIO引脚、串口参数和中断
 */
void Usart0_init(void) {
    /* GPIO配置参数 */
    /* TX引脚配置参数 */
    uint32_t usart_tx_rcu = RCU_GPIOA;
    uint32_t usart_tx_port = GPIOA;
    uint32_t usart_tx_pin = GPIO_PIN_9;
    uint32_t usart_tx_af = GPIO_AF_7;
    
    /* RX引脚配置参数 */
    uint32_t usart_rx_rcu = RCU_GPIOA;
    uint32_t usart_rx_port = GPIOA;
    uint32_t usart_rx_pin = GPIO_PIN_10;
    uint32_t usart_rx_af = GPIO_AF_7;
    
    /* 串口配置参数 */
    uint32_t usart_rcu = RCU_USART0;
    uint32_t usart = USART0;
    uint32_t usart_irq = USART0_IRQn;
    uint32_t usart_param_baudrate = 115200;    /* GPIO配置 - TX引脚(PA9) */
    rcu_periph_clock_enable(usart_tx_rcu);
    gpio_mode_set(usart_tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_tx_pin);
    gpio_output_options_set(usart_tx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_tx_pin);
    gpio_af_set(usart_tx_port, usart_tx_af, usart_tx_pin);
    
    /* GPIO配置 - RX引脚(PA10) */
    rcu_periph_clock_enable(usart_rx_rcu);
    gpio_mode_set(usart_rx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_rx_pin);
    gpio_output_options_set(usart_rx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_rx_pin);
    gpio_af_set(usart_rx_port, usart_rx_af, usart_rx_pin);

    /* 串口初始化 */
    rcu_periph_clock_enable(usart_rcu);
    usart_deinit(usart);
    
    /* 串口参数配置 */
    usart_baudrate_set(usart, usart_param_baudrate);  /* 波特率设置 */
    usart_parity_config(usart, USART_PM_NONE);        /* 无校验 */
    usart_word_length_set(usart, USART_WL_8BIT);      /* 8位数据位 */
    usart_stop_bit_set(usart, USART_STB_1BIT);        /* 1位停止位 */
    usart_data_first_config(usart, USART_MSBF_LSB);   /* 小端模式 */

    /* 发送接收配置 */
    usart_transmit_config(usart, USART_TRANSMIT_ENABLE);  /* 使能发送 */
    usart_receive_config(usart, USART_RECEIVE_ENABLE);    /* 使能接收 */
    
    /* 接收中断配置 */
    nvic_irq_enable(usart_irq, 2, 2);
    usart_interrupt_enable(usart, USART_INT_RBNE);    /* 接收缓冲区非空中断 */
    usart_interrupt_enable(usart, USART_INT_IDLE);    /* 空闲中断 */

    /* 使能串口 */
    usart_enable(usart);
}

/**
 * @brief 发送单个字节
 * @param data 要发送的数据
 */
void Usart0_send_byte(uint8_t data) {
    /* 通过USART发送 */
    usart_data_transmit(USART0, data);
    /* 等待发送完成 */
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

/**
 * @brief 发送指定长度的数据
 * @param data 数据指针
 * @param len  数据长度
 */
void Usart0_send_data(uint8_t* data, uint32_t len) {
    while(data && *data && len) {
        Usart0_send_byte(*data);
        data++;
        len--;
    }
}

/**
 * @brief 发送字符串
 * @param data 以'\0'结尾的字符串
 */
void Usart0_send_string(uint8_t* data) {
    /* 发送直到遇到'\0'结束符 */
    while(data && *data) {
        Usart0_send_byte(*data);
        data++;
    }
}

/**
 * @brief 重定向printf函数
 */
int fputc(int ch, FILE *f) {
    Usart0_send_byte((uint8_t)ch);
    return ch;
}

/**
 * @brief USART0中断处理函数
 * @note  处理接收数据和空闲中断
 */
void USART0_IRQHandler(void) {
    /* 接收缓冲区非空中断处理 */
    if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
        /* 清除中断标志 */
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        
        /* 接收数据并存入缓冲区 */
        uint8_t data = usart_data_receive(USART0);
        rx_buffer[rx_cnt++] = data;
        
        /* 防止缓冲区溢出 */
        if(rx_cnt >= RX_BUFFER_LEN) {
            rx_cnt = 0;
        }
    }
    
    /* 空闲中断处理 - 一帧数据接收完成 */
    if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
        /* 读取数据寄存器清除空闲中断标志 */
        usart_data_receive(USART0);
        
        /* 添加字符串结束符 */
        rx_buffer[rx_cnt] = '\0';
        
        /* 调用接收回调处理接收到的数据 */
        //Usart0_on_reon_recv(rx_buffer, rx_cnt);
        
        /* 清空接收计数器，准备下一次接收 */
        rx_cnt = 0;
    }
}

// void Usart0_on_recv(uint8_t* data, uint32_t len) {

//     printf("recv: %s\r\n", data);
// 	//	手动触发中断
// 	exti_software_interrupt_enable(EXTI_0);
// }