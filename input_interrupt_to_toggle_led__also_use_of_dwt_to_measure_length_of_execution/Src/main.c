#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN				   (1U<<0)
#define PIN5				   (1U<<5)

#define LED				   		PIN5

// we want an interrupt to occur when we press the push button
// push button is connected to PC13

static void exti_callback(void);
volatile uint32_t if_execution_time = 0;
char uart_buffer[50]; // Buffer to hold the UART output string


// Example implementation of UART_Transmit (modify according to your UART setup)
void UART_Transmit(const char* str)
{
    while (*str)
    {
        // Wait until the transmit data register is empty
        while (!(USART2->SR & USART_SR_TXE));
        // Transmit the character
        USART2->DR = *str++;
    }
}

void DWT_Init(void)
{
  // Enable TRC
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  // Enable the cycle counter
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

int main(void)
{

	/*1. Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/*2. Set PA5 as output pin */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);


	DWT_Init();
	pc13_exti_init();
	uart2_tx_init();

	while(1)
	{

	}
}

//void EXTI15_10_IRQHandler(void) { // name has to be this. each interrupt has a specific name for its handler
//	// this does EXTI interrupt for lines 15-10 so need to specify what line we are using
//	if((EXTI->PR & LINE13)!=0) // if trigger is one then it means it occurred
//	{
//		/*Clear PR flag*/
//		// this is to re-arm the flag
//		EXTI->PR |=LINE13;
//		//Do something...
//		GPIOA->ODR ^=LED;
//	}
//
//}


void EXTI15_10_IRQHandler(void)
{
  uint32_t start, end;

  // Measure the start time
  /* doing:
   * start = DWT->CYCCNT;
   * end = DWT->CYCCNT;
   * takes a certain amount of clock cycles without any tasks inside. must check and subtract from the total time*/
  start = DWT->CYCCNT;

  EXTI->PR |= LINE13;
  GPIOA->ODR ^= LED;

  // Measure the end time
  end = DWT->CYCCNT;

  // Calculate the execution time
  if_execution_time = end - start;

}
