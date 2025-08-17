#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f4xx.h"

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);


#define SR_RXNE			(1U<<5) // this was taken out of the .c file and put here because it needs to be used in the main.c too so this is #include in both of them
#define HISR_TCIF6		(1U<<21)
#define HIFCR_CTCIF6    (1U<<21)


#endif /* UART_H_ */
