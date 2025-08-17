//Where is the led connected?
//Port? A
//Pin? 5

#include<stdint.h>

// this is to start with finding boundary addresses for the GPIO and RCC
#define PERIPH_BASE						(0x40000000UL) // base means starting point of the boundary address
#define AHB1PERIPH_OFFSET 				(0x00020000UL) //this is the same as 0x20000UL because you can take left 0's away since it doesnt change position of the other numbers
#define AHB1PERIPH_BASE					(PERIPH_BASE + AHB1PERIPH_OFFSET)

// this is to get GPIOA boundary address
#define GPIOA_OFFSET					(0x0000UL) // this is same as 0x00000000; no offset because GPIOA is the first part of the AHB1PERIPH
#define GPIOA_BASE						(AHB1PERIPH_BASE + GPIOA_OFFSET)

// this is to get RCC boundary address
#define RCC_OFFSET						(0x3800UL)
#define RCC_BASE						(AHB1PERIPH_BASE + RCC_OFFSET)


// this is how to make clock work
#define GPIOAEN							(1U<<0) // we are changing the bit 0 of the 32 bits of the RCC AHB1 PERIPH clock enable to 1

// will be using the GPIOA ODR. When set to 0, it is off, when set to 1 it is on
#define PIN5							(1U<<5)
#define LED_PIN							PIN5 // to make it more readable, making variable LED_PIN



//#define __IO volatile // _IO just means volatile in another language so this is used

//typedef struct // if we are only using a specific amount of the following, we still need to keep the order since each occupies specific boundary address
//{ // each member in struct is 32 bits so it occupies for 4 bytes
//	__IO uint32_t MODER;		/*!< GPIO port mode register,					Address offset: 0x00		*/
//	__IO uint32_t OTYPER;		/*!< GPIO port output type register,			Address offset: 0x04		*/
//	__IO uint32_t OSPEEDR;		/*!< GPIO port output speed register,			Address offset: 0x08		*/
//	__IO uint32_t PUPDR;		/*!< GPIO port puul-up/pull-down register,		Address offset: 0x0C		*/
//	__IO uint32_t IDR;			/*!< GPIO port input data register,				Address offset: 0x10		*/
//	__IO uint32_t ODR;			/*!< GPIO port output data register,			Address offset: 0x14		*/
//	__IO uint32_t BSRR;			/*!< GPIO port bit set/reset register,			Address offset: 0x18		*/
//	__IO uint32_t LCKR;			/*!< GPIO port configuration lock register,		Address offset: 0x1C		*/
//	__IO uint32_t AFR[2];		/*!< GPIO alternate function register,			Address offset: 0x20-0x24	*/
//}GPIO_TypeDef;

// this struct is for GPIOA
typedef struct // to keep same order and not miss any parts, can make dummy variable for the ones we do not use. for example, we are only using oder and odr, so the 4 in the middle are not needed, can make dummy array if size 4
{
	volatile uint32_t MODER;		/*!< GPIO port mode register,					Address offset: 0x00		*/
	volatile uint32_t DUMMY[4];		// need to keep the space for other registers
	volatile uint32_t ODR;			/*!< GPIO port output data register,			Address offset: 0x14		*/
}GPIO_TypeDef;

// this struct is for RCC
typedef struct // to keep same order and not miss any parts, can make dummy variable for the ones we do not use. for example, we are only using oder and odr, so the 4 in the middle are not needed, can make dummy array if size 4
{
	volatile uint32_t DUMMY[12];		// need to keep the space for other registers
	volatile uint32_t AHB1ENR;			/*!< RCC AHB1 peripheral clock register,	Address offset: 0x14		*/
}RCC_TypeDef;


// this is getting boundary addresses from the original defines into the struct so that each part of the struct knows what its boundary address is
#define RCC								((RCC_TypeDef*) RCC_BASE)
#define GPIOA							((GPIO_TypeDef*) GPIOA_BASE)


int main(void)
{
	/*1. Enable clock access to GPIOA */
	// use |= below because then it will not change any of the other bits except for  the specific one that we want to change. in this case we want to change pin 0 to 1 and leave the rest of the bits the same as they were
	// RCC_AHB1EN_R |= GPIOAEN;

	RCC->AHB1ENR |= GPIOAEN;

	/*2. Set PA5 as output pin */
//	GPIOA_MODE_R |= (1U<<10);	// set bit 10 to 1
//	GPIOA_MODE_R &=~(1U<<11); // sets bit 11 to 0. ~ is negation

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER  &=~(1U<<11);

	while(1)
	{
//		/*3. Set PA5 high */ // outputting 1 at pin 5
//		GPIOA_OD_R |= LED_PIN;

		/*4. Experiment 2: toggle PA5*/ // ^ is the toggle operator
		GPIOA->ODR ^= LED_PIN;

		for(int i = 0; i<100000;i++){}
	}
}
