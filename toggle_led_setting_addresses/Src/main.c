//Where is the led connected?
//Port? A
//Pin? 5

#define PERIPH_BASE						(0x40000000UL) // base means starting point of the boundary address
// This is the offset because when you add offset to start of
//peripheral base, you get the start of the GPIOA which we found from its boundary address
#define AHB1PERIPH_OFFSET 				(0x00020000UL) //this is the same as 0x20000UL because you can take left 0's away since it doesnt change position of the other numbers
#define AHB1PERIPH_BASE					(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET					(0x0000UL) // this is same as 0x00000000; no offset because GPIOA is the first part of the AHB1PERIPH

#define GPIOA_BASE						(AHB1PERIPH_BASE + GPIOA_OFFSET)
// RCC in charge of enabling each bus to transport clock; resetting clock control
#define RCC_OFFSET						(0x3800UL)
#define RCC_BASE						(AHB1PERIPH_BASE + RCC_OFFSET)

#define AHB1EN_R_OFFSET					(0X30UL)	// use address offset from RCC_AHB1 enable
// to use the following as a register properly, we need type cast this to a volatile unsigned int pointer
#define RCC_AHB1EN_R					(*(volatile unsigned int *) (RCC_BASE + AHB1EN_R_OFFSET))

//this is how to register -- moder = mode register
#define MODE_R_OFFSET					(0X00UL)
#define GPIOA_MODE_R					(*(volatile unsigned int *) (GPIOA_BASE + MODE_R_OFFSET)) // all things done to GPIOA are now going to be from an offset from GPIOA_BASE

/* we are interested in pin 5, so that means we will use MODER5. Each MODER# uses two bits so that 4 different configurations can be done; changes the function of pin # from input, to output, to alternative function, to analog
 * this the mode register.
 *  (1U<<10)	// set bit 10 to 1
 *  &=~(1U<<11) // sets bit 11 to 0. ~ is negation
 *  this 01 would make pin 5 function as an output pin
 */

//since we want output of an led, we will be using the GPIO ODR = output data register
// 32 bits, last 16 are reserved, first 16 are used for all pin0 -> pin 15. in this we need pin 5 so ODR5 will be used
#define OD_R_OFFSET						(0x14UL)
#define GPIOA_OD_R						(*(volatile unsigned int *) (GPIOA_BASE + OD_R_OFFSET))



// this is how to make clock work
#define GPIOAEN							(1U<<0) // we are changing the bit 0 of the 32 bits of the RCC AHB1 PERIPH clock enable to 1
// changing 0b 0000 0000 0000 0000 0000 0000 0000 0000
// to  		0b 0000 0000 0000 0000 0000 0000 0000 0001
// the bit 0 corresponds to GPIO EN, bit 1 to GPIOB EN, etc.


// will be using the GPIOA ODR. When set to 0, it is off, when set to 1 it is on
#define PIN5							(1U<<5)
#define LED_PIN							PIN5 // to make it more readable, making variable LED_PIN





int main(void)
{
	/*1. Enable clock access to GPIOA*/
	// use |= below because then it will not change any of the other bits except for  the specific one that we want to change. in this case we want to change pin 0 to 1 and leave the rest of the bits the same as they were
	RCC_AHB1EN_R |= GPIOAEN;
	/*2. Set PA5 as output pin*/
	GPIOA_MODE_R |= (1U<<10);	// set bit 10 to 1
	GPIOA_MODE_R &=~(1U<<11); // sets bit 11 to 0. ~ is negation

	while(1)
	{
//		/*3. Set PA5 high*/ // outputting 1 at pin 5
//		GPIOA_OD_R |= LED_PIN;

		/*4. Experiment 2: toggle PA5*/ // ^ is the toggle operator
		GPIOA_OD_R ^= LED_PIN;// happens too fast so we make a for loop to wait
		for(int i = 0; i<100000;i++){}
	}
}
