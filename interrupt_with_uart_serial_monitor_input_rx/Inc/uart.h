#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f4xx.h"

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);

#define SR_RXNE			(1U<<5) // this was taken out of the .c file and put here because it needs to be used in the main.c too so this is #include in both of them

#endif /* UART_H_ */
