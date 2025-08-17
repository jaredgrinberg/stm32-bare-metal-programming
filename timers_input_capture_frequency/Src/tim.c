#include "stm32f4xx.h"

#define TIM2EN			(1U<<0)
#define CR1_CEN			(1U<<0)

#define TIM2_TOGGLE_1	(1U<<4)
#define TIM2_TOGGLE_2	(1U<<5)
#define TIM2_TOGGLE_3	(1U<<6)

#define CCER_CC1E		(1U<<0)

#define GPIOAEN			(1U<<0)

#define AFR5_TIM		(1U<<20)

#define AFR6_TIM		(1U<<25)

#define TIM3EN			(1U<<1)

#define CCER_CC1S		(1U<<0)

void tim2_1hz_init(void)
{
	/* Enable clock access to timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 1600 - 1; // 16 000 000 / 1 600 = 10 000

	/* Set auto reload value */
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/* Clear counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1  = CR1_CEN;

}


void tim2_PA5_output_compare(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set PA5 mode to alternate function */
	// set bits to 10 for alternate
	GPIOA->MODER &=~ (1U<<10);
	GPIOA->MODER = (1U<<11);


	/* Set PA5 function type to TIM2_CH1 which is AF01 */
	// alternate function register low because low pin and its bits 20-23, AF1 is 0001
	GPIOA->AFR[0] |= AFR5_TIM;


	/* Enable clock access to timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 1600 - 1; // 16 000 000 / 1 600 = 10 000

	/* Set auto reload value */
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1


	/* Set output compare toggle mode */
	// look for TIMx_CCMR (capture/compare mode register)
	// using channel 1 below so we are looking for OC1M which is bits 4-6. set to 011 then does toggle
	TIM2->CCMR1 = TIM2_TOGGLE_1 | TIM2_TOGGLE_2;
	TIM2->CCMR1 &=~ TIM2_TOGGLE_3;


	/* Enable tim2 ch1 in compare mode */
	// we are doing tim2 ch1 because we want to toggle the LED which is connected to PA5. in alternate function table, pa5 does tim2 ch1
	// in TIMx_CCER and using CC1E for output enable for channel 1
	TIM2->CCER |= CCER_CC1E;


	/* Clear counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1  = CR1_CEN;

}

void tim3_PA6_input_capture(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set PA5 mode to alternate function */
	// set bits to 10 for alternate
	GPIOA->MODER &=~ (1U<<12);
	GPIOA->MODER = (1U<<13);


	/* Set PA6 function type to TIM3_CH1 which is AF02 */
	// alternate function register low because low pin and its bits 24-27, AF2 is 0010
	GPIOA->AFR[0] |= AFR6_TIM;


	/* Enable clock access to timer3 */
	RCC->APB1ENR |= TIM3EN;

	/* Set prescaler value */
	TIM3->PSC = 16000 - 1; // 16 000 000 / 16 000 = 1 000

	/* Set CH1 to input capture */
	// still using TIMx_CCMR, it is used for capture and compare
	// now need bits 0 and 1; need CC1S; setting to 01
	TIM3->CCMR1	= CCER_CC1S;

	/* Set CH1 to capture at rising edge */
	// now CCER for capture compare enable register, CC1E
	TIM3->CCER = CCER_CC1E;

	/* Enable timer */
	TIM3->CR1  = CR1_CEN;

}

