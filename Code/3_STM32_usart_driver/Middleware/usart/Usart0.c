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
	
    // GPIO��AFIO
    // GPIO��ʼ����TX:PA9  RX: PA10
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

    // ���ڳ�ʼ��
    // ʱ��
		RCC_APB2PeriphClockCmd(usart_rcu, ENABLE);
    USART_DeInit(usart);
    // ���ڵĲ������ã������ʣ�ֹͣλ...����С��
		USART_InitTypeDef param;
		USART_StructInit(&param);
		param.USART_BaudRate = usart_param_baudrate;
		USART_Init(usart, &param);

		//ͨ���жϽ��н���
		 // �����ж�����
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
		 //ͨ��USART����
    USART_SendData(USART1, data);
    //�жϻ������Ƿ��Ѿ�����
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
		//����: 1.dataָ�벻Ϊ��  2.���͵����ݲ���\0�������
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
	//1. �жϺ���ֻ��1��
	//2. �����жϵı���Ƕ��
	
	//��Ҫ������ĸ���Ǵ������ж�
		
	if(SET == USART_GetITStatus(USART1, USART_IT_RXNE)) {
			//���յ�������
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
			// �����ݸ㵽��
			uint8_t data = USART_ReceiveData(USART1);
			rx_buffer[rx_cnt++] = data;
		
			//send_data(data);
			// printfִ�к�ʱ�� ���ܸ��ػ�
			//printf("hello\r\n");
			//printf("%s", &data);
	}
	if (SET == USART_GetITStatus(USART1, USART_IT_IDLE)) {
			//����
			USART_ReceiveData(USART1);
			//usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
			rx_buffer[rx_cnt] = '\0';
		
			//�����������������, �߼���ʲô
			// printf("%s", rx_buffer);
			Usart0_on_recv(rx_buffer, rx_cnt);
		
			//��������
			rx_cnt = 0;
	}
		
}