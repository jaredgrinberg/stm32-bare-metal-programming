#include <stdio.h>
#include <stdint.h> // using unit32_t so need this
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN			(1U<<0)
#define PIN5			(1U<<5)
#define LED_PIN			PIN5

// we are trying to toggle the GPIO pin like in project 12 except without the use of a toggle ^=
// we are directly connecting PA5 to the timer so that while look is empty and it toggles it when a certain period occurs

int main(void)
{

	tim2_PA5_output_compare();


	while(1)
	{

	}
}
