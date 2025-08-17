#include <stdio.h>
#include <stdint.h> // using unit32_t so need this
#include "stm32f4xx.h"
#include "uart.h"

int main(void)
{

	uart2_tx_init();


	while(1)
	{
		printf("Hello from STM32F4........\n\r");

	}
}

