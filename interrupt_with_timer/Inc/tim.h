/*
 * tim.h
 *
 *  Created on: May 27, 2024
 *      Author: jared
 */

#ifndef TIM_H_
#define TIM_H_

void tim2_1hz_init(void);

#define SR_UIF  (1U<<0) // needed in main.c
void tim2_1hz_interrupt_init(void);

#endif /* TIM_H_ */
