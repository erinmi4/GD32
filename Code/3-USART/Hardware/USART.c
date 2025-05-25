#include "USART.h"

//定义全局变量
uint8_t rx_buffer[RX_BUFFER_SIZE];//接收缓冲区
uint32_t rx_cnt = 0;//接收计数

//配置串口0
void usart_config(void){
    //设置TX
    uint32_t TX_rcu = RCU_GPIOA; //TX引脚对应的GPIO端口
    uint32_t TX_pin = GPIO_PIN_9; //TX引脚对应的GPIO引脚
    uint32_t TX_af = GPIO_AF_7; //TX引脚复用功能
    uint32_t TX_GPIO_port = GPIOA; //TX引脚对应的GPIO端口
    //设置RX
    uint32_t RX_rcu = RCU_GPIOA; //RX引脚对应的GPIO端口
    uint32_t RX_pin = GPIO_PIN_10; //RX引脚对应的GPIO引脚
    uint32_t RX_af = GPIO_AF_7; //RX引脚复用功能
    uint32_t RX_GPIO_port = GPIOA; //RX引脚对应的GPIO端口
    
    uint32_t usart_periph = USART0; //串口0
    //参数配置
    uint32_t usart_baudrate = 115200; //波特率
    uint32_t usart_parity = USART_PM_NONE; //无奇偶校验
    uint32_t usart_word_length = USART_WL_8BIT; //数据位8位
    uint32_t usart_stop_bit = USART_STB_1BIT; //停止位1位
    uint32_t usart_data_first = USART_MSBF_LSB; //小端模式
    uint32_t usart_transmit = USART_TRANSMIT_ENABLE; //使能发送
    uint32_t usart_receive = USART_RECEIVE_ENABLE; //使能接收
    uint32_t usart_nvic_irq = USART0_IRQn; //串口0中断

    //设置TX
    rcu_periph_clock_enable(TX_rcu); //使能GPIOA时钟
    gpio_mode_set(TX_GPIO_port, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_pin); //设置TX引脚为复用模式
    gpio_output_options_set(TX_GPIO_port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TX_pin); //设置TX引脚为推挽输出，速度50MHz
    gpio_af_set(TX_GPIO_port, TX_af, TX_pin); //设置TX引脚为复用功能7
    //设置RX
    rcu_periph_clock_enable(RX_rcu); //使能GPIOA时钟
    gpio_mode_set(RX_GPIO_port, GPIO_MODE_AF, GPIO_PUPD_NONE, RX_pin); //设置RX引脚为复用模式
    gpio_output_options_set(RX_GPIO_port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, RX_pin); //设置RX引脚为推挽输出，速度50MHz
    gpio_af_set(RX_GPIO_port, RX_af, RX_pin); //设置TX引脚为复用功能7
    //配置串口
    rcu_periph_clock_enable(RCU_USART0); //使能USART0时钟
    usart_deinit(usart_periph); //复位串口0
    usart_baudrate_set(usart_periph, usart_baudrate); //设置波特率为115200
    usart_parity_config(usart_periph, usart_parity); //设置无奇偶校验
    usart_word_length_set(usart_periph, usart_word_length); //设置数据位为8位
    usart_stop_bit_set(usart_periph, usart_stop_bit); //设置停止位为1位
    usart_data_first_config(usart_periph, usart_data_first); //小端模式

    usart_transmit_config(usart_periph, usart_transmit); //使能发送
    usart_receive_config(usart_periph, usart_receive); //使能接收
    //中断配置
    nvic_irq_enable(USART0_IRQn, 2, 2); //使能串口0中断
    usart_interrupt_enable(usart_periph, USART_INT_RBNE); //使能接收中断
    usart_interrupt_enable(usart_periph, USART_INT_IDLE); //使能闲置中断
    //使能串口
    usart_enable(usart_periph);
}

//发送数据
void usart_send_data(uint8_t data){
    usart_data_transmit(USART0, data); //发送数据
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)); //等待发送缓冲区空
}

void usart_send_string(char *str){
    while(str && *str != '\0'){ // 判断字符串是否为空,并且判断是否到达字符串结尾
        usart_send_data(*str); //发送数据
        str++;
    }
}

int fputc(int ch, FILE *f){
    usart_send_data((uint8_t)ch); //发送数据
    return ch;
}

void USART0_IRQHandler(void){
    //受到数据，打印hello   
    if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        uint8_t data = usart_data_receive(USART0); //接收数据
        rx_buffer[rx_cnt++] = data; //存入接收缓冲区
        if(rx_cnt >= RX_BUFFER_SIZE){ //如果接收缓冲区满了
            rx_cnt = 0; //清空接收计数
        }
    }
    //闲置中断
    if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))
    {
        usart_data_receive(USART0); //清除闲置中断标志
        //可以在这里处理闲置事件
        	rx_buffer[rx_cnt] = '\0';//防止脏数据
		
			//处理缓冲区里面的内容
			printf("%s", rx_buffer);
		
			//清理缓冲区
			rx_cnt = 0;
    }
    
}