#ifndef UART_H_
#define UART_H_
#include <stdint.h> // using unit32_t so need this
#include "stm32f4xx.h"

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);


#endif /* UART_H_ */
