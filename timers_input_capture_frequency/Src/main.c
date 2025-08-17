#include <stdio.h>
#include <stdint.h> // using uint32_t so need this
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

// we are able to capture the frequency of the input using the timer
// still using what we did in project 13, but now we want to capture into another timer pin the toggle rate from the output compare
// need to connect pa5 to a separate timer channel to read rate

int timestamp = 0;

// need to connect wire from pa5 to pa6 so that we can read the signal

int main(void)
{

	tim2_PA5_output_compare(); // this is signal source
	tim3_PA6_input_capture(); // this is signal capture

	while(1)
	{
		// wait until edge is captured
		while(!(TIM3->SR & SR_CC1IF)){}

		// read value
		timestamp = TIM3->CCR1;
	}
}
