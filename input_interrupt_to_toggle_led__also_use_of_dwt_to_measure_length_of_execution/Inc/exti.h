/*
 * exti.h
 *
 *  Created on: May 27, 2024
 *      Author: jared
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"
void pc13_exti_init(void);

#define LINE13		(1U<<13)

#endif /* EXTI_H_ */
