#include "Usart0.h"


#define RX_BUFFER_LEN 		1024
static uint8_t 	rx_buffer[RX_BUFFER_LEN];
static uint32_t rx_cnt = 0;


void Usart0_init() {
		uint32_t usart_tx_rcu = RCU_GPIOA;
		uint32_t usart_tx_port = GPIOA;
		uint32_t usart_tx_pin = GPIO_PIN_9;
		uint32_t usart_tx_af = GPIO_AF_7;
	
		uint32_t usart_rx_rcu = RCU_GPIOA;
		uint32_t usart_rx_port = GPIOA;
		uint32_t usart_rx_pin = GPIO_PIN_10;
		uint32_t usart_rx_af = GPIO_AF_7;
	
		uint32_t usart_rcu = RCU_USART0;
		uint32_t usart = USART0;
		uint32_t usart_irq = USART0_IRQn;
		
		uint32_t usart_param_baudrate = 115200;
	
    // GPIO和AFIO
    // GPIO初始化：TX:PA9  RX: PA10
    ////// TX
    rcu_periph_clock_enable(usart_tx_rcu);
    gpio_mode_set(usart_tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_tx_pin);
    gpio_output_options_set(usart_tx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_tx_pin);
    gpio_af_set(usart_tx_port, usart_tx_af, usart_tx_pin);
    ////// RX
    rcu_periph_clock_enable(usart_rx_rcu);
    gpio_mode_set(usart_rx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usart_rx_pin);
    gpio_output_options_set(usart_rx_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, usart_rx_pin);
    gpio_af_set(usart_rx_port, usart_rx_af, usart_rx_pin);

    // 串口初始化
    // 时钟
    rcu_periph_clock_enable(usart_rcu);
    usart_deinit(usart);
    // 串口的参数配置：波特率，停止位...，大小端
    usart_baudrate_set(usart, usart_param_baudrate);
    usart_parity_config(usart, USART_PM_NONE);
    usart_word_length_set(usart, USART_WL_8BIT);
    usart_stop_bit_set(usart, USART_STB_1BIT);
    // 0000 0000
    usart_data_first_config(usart, USART_MSBF_LSB);

    // 开启发送
    usart_transmit_config(usart, USART_TRANSMIT_ENABLE);
		
		// 开启接收
		usart_receive_config(usart, USART_RECEIVE_ENABLE);
		//通过中断进行接收
		 // 接收中断配置
    nvic_irq_enable(usart_irq, 2, 2);
    // usart int rbne
    usart_interrupt_enable(usart, USART_INT_RBNE);
    usart_interrupt_enable(usart, USART_INT_IDLE);

    usart_enable(usart);
}

void Usart0_send_byte(uint8_t data) {
		 //通过USART发送
    usart_data_transmit(USART0, data);
    //判断缓冲区是否已经空了
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

void Usart0_send_data(uint8_t* data, uint32_t len) {
		while(data && *data && len) {
        Usart0_send_byte((uint8_t)(*data));
        data++;
				len--;
    }
}


void Usart0_send_string(uint8_t* data) {
		//满足: 1.data指针不为空  2.发送的数据不是\0结束标记
    while(data && *data) {
        Usart0_send_byte((uint8_t)(*data));
        data++;
    }
}

int fputc(int ch, FILE *f) {
    Usart0_send_byte((uint8_t)(ch));
    return ch;
}

void USART0_IRQHandler(){
	//1. 中断函数只有1个
	//2. 触发中断的标记是多个
	
	//需要搞清楚哪个标记触发的中断
	if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
			//接收到数据了
			usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
		
			// 把数据搞到手
			uint8_t data = usart_data_receive(USART0);
			rx_buffer[rx_cnt++] = data;
		
			//send_data(data);
			// printf执行耗时， 不能干重活
			//printf("hello\r\n");
			//printf("%s", &data);
	}
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
			//闲置
			usart_data_receive(USART0);
			//usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
			rx_buffer[rx_cnt] = '\0';
		
			//处理缓冲区里面的内容, 逻辑是什么
			// printf("%s", rx_buffer);
			Usart0_on_recv(rx_buffer, rx_cnt);
		
			//清理缓冲区
			rx_cnt = 0;
	}
		
}