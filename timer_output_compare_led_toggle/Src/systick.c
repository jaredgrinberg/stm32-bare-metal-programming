#include "stm32f4xx.h"


#define SYSTICK_LOAD_VAL		16000 // want ms delay and since default clock is 16mhz we just divide by 1000

#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<2)
#define CTRL_COUNTFLAG			(1U<<16)

void systickDelayMs(int delay)
{
	// to enable systick timer look at systick control and status register where bit 0 is the enable bit
	// bit 2 chooses clock source where 0 is external clock and 1 is processor clock
	// countflag is bit 16 which returns 1 if timer counted to 0 since last time read


	/* Configure systick */
	/* Reload with number of clocks per millisecond */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Clear systick current value register */
	SysTick->VAL = 0;

	/* Enable systick and select internal clock source */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC; // do both together

	for(int i = 0; i < delay; i++){

		// wait until the countflag is set
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}

	SysTick->CTRL = 0;

}
