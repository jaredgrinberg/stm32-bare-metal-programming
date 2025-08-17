#include <stdio.h>
#include <stdint.h> // using unit32_t so need this
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"


uint32_t sensor_value;



int main(void)
{
	uart2_tx_init(); // initialize uart
	pa1_adc_init(); // initialize adc
	start_conversion(); //

	while(1)
	{

		sensor_value = adc_read();
		printf("Sensor value : %d \n\r", (int)sensor_value);
	}
}
