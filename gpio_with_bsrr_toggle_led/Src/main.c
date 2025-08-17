#include "stm32f4xx.h"

#define GPIOAEN			(1U<<0)
#define PIN5			(1U<<5)
#define LED_PIN			PIN5


int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	while(1)
	{
		GPIOA->BSRR = LED_PIN; // this is for BS5 to set
		for(int i = 0; i < 100000; i++){}

		GPIOA->BSRR = (1U<<21); // this is for BR5 to reset
		for(int i = 0; i < 100000; i++){}
	}
}
