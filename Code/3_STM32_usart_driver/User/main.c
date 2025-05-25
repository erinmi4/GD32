
#include "main.h"
#include "systick.h"
#include "Usart0.h"


void Usart0_on_recv(uint8_t* data, uint32_t len) {

	printf("recv: %s\r\n", data);
}

int main(void)
{
    systick_config();
		Usart0_init();
		uint8_t cnt = 0;
    while(1) {
				delay_1ms(1000);
			
				printf("hello %d\r\n", cnt++);
    }
}

