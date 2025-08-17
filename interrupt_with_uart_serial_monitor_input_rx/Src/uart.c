#include "uart.h"



#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_FREQ		16000000 // default clock is 16Mhz
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE	115200

#define CR1_TE			(1U<<3) // this enables TE
#define CR1_RE			(1U<<2) // this enables RE
#define CR1_UE			(1U<<13) // this enables UART Enable

#define SR_TXE			(1U<<7) // this enables TX Enable
#define CR1_RXNEIE		(1U<<5) // this enables interrupt for RXNE


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_write(int ch);

int __io_putchar( int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_rxtx_init(void)
{
	/*Configure uart gpio pin*/
	/*Enable clock access to gpioa*/
	// need gpioa because we need PA2 pin
	// we know that clock access is rcc and we know that gpioa is the ahb1 bus
	RCC->AHB1ENR |= GPIOAEN; //enables gpioa



	//THE NEXT PART IS FOR TX



	/*Set PA2 mode to alternate function mode*/
	// we are using pin 2 so its moder2 which is bits 5 and 4. we know that 10 is alternate function mode so we need to input that in
	// using pin 2 because it is in the alternate function mapping for USART2_TX
	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);

	/*Set PA2 alternate function type of UART_TX (AF07)*/
	// need AF07 because it is in the alternate function mapping for using USART2_TX which we need
	// there is 4 bits per AFRL, meaning that there if AFRL0 - AFRL7. the numbers represent all pins of that number. in this case we use AFRL2
	// we need AF07 so just input binary 7 into the 4 bits allocated for AFRL2 which is 8-11
	// there is no AFRL or AFRH but AFR is an array where [0] is for low and [1] is for high
	// AFRL is for pins 0-7, AFRH is for pins 8-15
	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<8);



	//THE NEXT PART IS FOR TX



	/*Set PA3 mode to alternate function mode*/
	// we are using pin 3 so its moder3 which is bits 7 and 6. we know that 10 is alternate function mode so we need to input that in
	// using pin 3 because it is in the alternate function mapping for USART2_RX
	GPIOA->MODER |= (1U<<7);
	GPIOA->MODER &=~(1U<<6);

	/*Set PA3 alternate function type of UART_RX (AF07)*/
	// need AF07 because it is in the alternate function mapping for using USART2_RX which we need
	// there is 4 bits per AFRL, meaning that there if AFRL0 - AFRL7. the numbers represent all pins of that number. in this case we use AFRL2
	// we need AF07 so just input binary 7 into the 4 bits allocated for AFRL2 which is 8-11
	// there is no AFRL or AFRH but AFR is an array where [0] is for low and [1] is for high
	// AFRL is for pins 0-7, AFRH is for pins 8-15
	GPIOA->AFR[0] &=~(1U<<15);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<12);






	/*Configure uart module*/
	/*Enable clock access to uart2*/
	// uart2 is in the APB1 bus
	RCC->APB1ENR |= UART2EN; // enables uart2

	/*Configure Baud Rate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);



	// THE FOLLOWING IS FOR BOTH TRANSMISSION AND RECEIVING



	/*Configure the transfer direction*/
	// reference manual USART control register CR. the transmitter enable is in CR1 so using CR1
	// want to enable the transmitter which is TE and receiver which is RE in the USART_CR1
	USART2->CR1 = (CR1_TE | CR1_RE); // no |= because want to set everything else to 0
	// does an OR operation to also include RE to enable both at the same time


	/*Enable uart module*/
	// enable uart is in the same place as transfer direction but pin 13
	USART2->CR1 |= CR1_UE; // do not want to completely clear everything since we just changed the TE to 1


}


void uart2_rx_interrupt_init(void)
{
	/****************Configure uart gpio pin***************/
	/*Enable clock access to gpioa */
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);


	/*Set PA3 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);




	/****************Configure uart module ***************/
	/*Enable clock access to uart2 */
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	/*Configure the transfer direction*/
	 USART2->CR1 =  (CR1_TE | CR1_RE);

	 /*Enable RXNE interrupt*/
	 USART2->CR1 |=CR1_RXNEIE;

	 /*Enable UART2 interrupt in NVIC*/
	 NVIC_EnableIRQ(USART2_IRQn);

	/*Enable uart module*/
	 USART2->CR1 |= CR1_UE;


}


void uart2_tx_init(void)
{
	/*Configure uart gpio pin*/
	/*Enable clock access to gpioa*/
	// need gpioa because we need PA2 pin
	// we know that clock access is rcc and we know that gpioa is the ahb1 bus
	RCC->AHB1ENR |= GPIOAEN; //enables gpioa

	/*Set PA2 mode to alternate function mode*/
	// we are using pin 2 so its moder2 which is bits 5 and 4. we know that 10 is alternate function mode so we need to input that in
	// using pin 2 because it is in the alternate function mapping for USART2_TX
	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);

	/*Set PA2 alternate function type of UART_TX (AF07)*/
	// need AF07 because it is in the alternate function mapping for using USART2_TX which we need
	// there is 4 bits per AFRL, meaning that there if AFRL0 - AFRL7. the numbers represent all pins of that number. in this case we use AFRL2
	// we need AF07 so just input binary 7 into the 4 bits allocated for AFRL2 which is 8-11
	// there is no AFRL or AFRH but AFR is an array where [0] is for low and [1] is for high
	// AFRL is for pins 0-7, AFRH is for pins 8-15
	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<8);



	/*Configure uart module*/
	/*Enable clock access to uart2*/
	// uart2 is in the APB1 bus
	RCC->APB1ENR |= UART2EN; // enables uart2

	/*Configure Baud Rate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	// reference manual USART control register CR. the transmitter enable is in CR1 so using CR1
	// want to enable the transmitter which is TE in the USART_CR1
	USART2->CR1 = CR1_TE; // no |= because want to set everything else to 0

	/*Enable uart module*/
	// enable uart is in the same place as transfer direction but pin 13
	USART2->CR1 |= CR1_UE; // do not want to completely clear everything since we just changed the TE to 1
}





char uart2_read(void)
{
	/* This time we need to make sure that the receive data register is not empty */
	while(!(USART2->SR & SR_RXNE)){} // will get stuck here until the and operation gives back a false

	/* Read data */
	return USART2->DR;
}





void uart2_write(int ch)
{
	/* Make sure that transmit data register is empty */
	// have the check USART_SR status register. we need TXE - transmit data register empty which is bit 7
	// do an AND operation between the status register and the bit you are interested in which is TXE
	while(!(USART2->SR & SR_TXE)){} // will get stuck here until the and operation gives back a false

	/* Write to transmit data register */

	USART2->DR = (ch & 0xFF); //DR stands for data register. perform AND operation with ch and 0xFF to make it 8 bit
}





static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{ // U means unsigned number
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
