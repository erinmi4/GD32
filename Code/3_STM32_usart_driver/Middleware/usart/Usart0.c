#include "Usart0.h"


#define RX_BUFFER_LEN 		1024
static uint8_t 	rx_buffer[RX_BUFFER_LEN];
static uint32_t rx_cnt = 0;


void Usart0_init() {
		uint32_t usart_tx_rcu = RCC_AHB1Periph_GPIOA;
		GPIO_TypeDef* usart_tx_port = GPIOA;
		uint32_t usart_tx_pin = GPIO_Pin_9;
		uint8_t  usart_tx_af_pin = GPIO_PinSource9;
		uint32_t usart_tx_af = GPIO_AF_USART1;
	
		uint32_t usart_rx_rcu = RCC_AHB1Periph_GPIOA;
		GPIO_TypeDef* usart_rx_port = GPIOA;
		uint32_t usart_rx_pin = GPIO_Pin_10;
		uint8_t  usart_rx_af_pin = GPIO_PinSource10;
		uint32_t usart_rx_af = GPIO_AF_USART1;
	
		uint32_t usart_rcu = RCC_APB2Periph_USART1;
		USART_TypeDef* usart = USART1;
		uint32_t usart_irq = USART1_IRQn;
		
		uint32_t usart_param_baudrate = 115200;
	
    // GPIO和AFIO
    // GPIO初始化：TX:PA9  RX: PA10
		GPIO_InitTypeDef GPIO_InitStructure;
    ////// TX
		RCC_AHB1PeriphClockCmd(usart_tx_rcu, ENABLE);
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = usart_tx_pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(usart_tx_port, &GPIO_InitStructure);
		GPIO_PinAFConfig(usart_tx_port, usart_tx_af_pin, usart_tx_af);
		
    ////// RX
    RCC_AHB1PeriphClockCmd(usart_rx_rcu, ENABLE);
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = usart_rx_pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(usart_rx_port, &GPIO_InitStructure);
		GPIO_PinAFConfig(usart_rx_port, usart_rx_af_pin, usart_rx_af);

    // 串口初始化
    // 时钟
		RCC_APB2PeriphClockCmd(usart_rcu, ENABLE);
    USART_DeInit(usart);
    // 串口的参数配置：波特率，停止位...，大小端
		USART_InitTypeDef param;
		USART_StructInit(&param);
		param.USART_BaudRate = usart_param_baudrate;
		USART_Init(usart, &param);

		//通过中断进行接收
		 // 接收中断配置
		NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = usart_irq;
		nvic.NVIC_IRQChannelPreemptionPriority = 2;
		nvic.NVIC_IRQChannelSubPriority = 2;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
    // usart int rbne
		USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
		USART_ITConfig(usart, USART_IT_IDLE, ENABLE);

		USART_Cmd(usart, ENABLE);
}

void Usart0_send_byte(uint8_t data) {
		 //通过USART发送
    USART_SendData(USART1, data);
    //判断缓冲区是否已经空了
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
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

void USART1_IRQHandler(){
	//1. 中断函数只有1个
	//2. 触发中断的标记是多个
	
	//需要搞清楚哪个标记触发的中断
		
	if(SET == USART_GetITStatus(USART1, USART_IT_RXNE)) {
			//接收到数据了
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
			// 把数据搞到手
			uint8_t data = USART_ReceiveData(USART1);
			rx_buffer[rx_cnt++] = data;
		
			//send_data(data);
			// printf执行耗时， 不能干重活
			//printf("hello\r\n");
			//printf("%s", &data);
	}
	if (SET == USART_GetITStatus(USART1, USART_IT_IDLE)) {
			//闲置
			USART_ReceiveData(USART1);
			//usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
			rx_buffer[rx_cnt] = '\0';
		
			//处理缓冲区里面的内容, 逻辑是什么
			// printf("%s", rx_buffer);
			Usart0_on_recv(rx_buffer, rx_cnt);
		
			//清理缓冲区
			rx_cnt = 0;
	}
		
}