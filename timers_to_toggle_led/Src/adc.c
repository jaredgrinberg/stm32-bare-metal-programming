#include "stm32f4xx.h"
#include "adc.h"


#define ADC1EN		(1U<<8)

#define GPIOAEN		(1U<<0)

#define ADC_CH1		(1U<<0)

#define ADC_SEQ_LEN_1	0x00

#define CR2_ADON	(1U<<0)

#define CR2_SWSTART	(1U<<30)

#define SR_EOC		(1U<<1)

#define CR2_CONT	(1U<<1)

/*adc configured with 3 channels
 * ch2, ch3, ch5
 *  order we want the adc to sample the channels
 *
 * first = ch5 - this would be SQ1 and would take up bits 0-4. make binary number 5 with these
 * second = ch2 - this would be SQ2 with binary 2
 * third = ch3 - etc.
 */



void pa1_adc_init(void)
{
	/* Configure the ADC pin */
	/* Enable clock access to ADC pin's port, which is GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Set the mode of PA1 to analog */
	// moder1 so pins 2 and 3
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);


	/* Configure the ADC module */
	/* Enable clock access to ADC module */
	RCC->APB2ENR |= ADC1EN;

	/* Configure ADC parameters in the following steps*/
	/* Conversion sequence start */
	// in the sequence register
	ADC1->SQR3 = ADC_CH1;

	/* Conversion sequence length */
	// sequence register 1 contains the regular channel sequence length, we are only doing one channel in bits numbered 20-23
	// there are 16 channels, 16 combinations with 4 bits, meaning 1 channel is 0000
	ADC1->SQR1 = ADC_SEQ_LEN_1; // since we want it 0000, we can just make everything in SQR1 0 since we are not using the other things in SQR1

	/* Enable ADC module */
	// ADC control register, looking for ADC on/off which is called ADON. it is bit 0 of ADC_CR3
	ADC1->CR2 |= CR2_ADON;
}



void start_conversion(void)
{
	/* Enable continuous conversion */
	ADC1->CR2 |= CR2_CONT; // continuous conversion

	/* Start the ADC conversion */
	// looking for SWSTART in the ADC_CR which is in CR2 bit 30
	ADC1->CR2 |= CR2_SWSTART;
}


uint32_t adc_read(void)
{
	/* Wait for ADC conversion */
	// looking for EOC (end of conversion) bit which is bit 1 of ADC SR; 1 means conversion is complete
	while(!(ADC1->SR & SR_EOC)){} // stuck here until conversion is true

	/* Read converted result */
	// returning the data register
	return (ADC1->DR);
}

