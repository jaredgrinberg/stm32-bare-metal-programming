/*
 * exti.c
 *
 *  Created on: May 27, 2024
 *      Author: jared
 */

#include "exti.h"


#define GPIOCEN		    (1U<<2)
#define SYSCFGEN		(1U<<14)

void pc13_exti_init(void)
{
	/*Disable global interrupts*/
	// this is not needed but it is good practice
	__disable_irq();

	/*Enable clock access for GPIOC*/
	RCC->AHB1ENR |=GPIOCEN;

	/*Set PC13 as input*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);


	/*Enable clock access to SYSCFG*/
	RCC->APB2ENR |=SYSCFGEN;

	/*Select PORTC for EXTI13*/
	// each EXTI# uses 4 bits for configuration
	// since we want exti13 we need to get exticr4 since thats where it is located. thus the 4th section of exticr is [3]
	SYSCFG->EXTICR[3] |=(1U<<5);

	/*Unmask EXTI13*/
	// interrupt mask register is IMR
	EXTI->IMR |=(1U<<13);

	/*Select falling edge trigger*/
	//FTSR is falling trigger select register
	EXTI->FTSR |=(1U<<13);
	EXTI->RTSR |=(1U<<13);

	/*Enable EXTI13 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn); // this number is the same as number 40. it is the symbolic name or some specific interrupt

	/*Enable global interrupts*/
	__enable_irq();


}


