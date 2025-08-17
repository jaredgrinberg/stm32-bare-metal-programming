#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "nau7802.h"


uint32_t force_value;

int main(void)
{

	nau7802_init();

	while(1)
	{
		nau7802_read_values(&force_value);
		char buffer[50];
		snprintf(buffer, sizeof(buffer), "Read %ld\r\n", (int32_t)(force_value) - 33950);
		UART_Print(buffer);
	}

}




