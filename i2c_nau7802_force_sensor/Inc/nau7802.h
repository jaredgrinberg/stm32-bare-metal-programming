/*
 * nau7802.h
 *
 *  Created on: May 27, 2024
 *      Author: jared
 */

#ifndef NAU7802_H_
#define NAU7802_H_

#include "i2c.h"
#include <stdint.h>

// NAU7802 Register Addresses
#define NAU7802_CTRL1         0x01
#define NAU7802_CTRL2         0x02
#define NAU7802_CTRL_ADC      0x15
#define NAU7802_CTRL_PWR      0x1C
#define NAU7802_PU_CTRL       0x00
#define NAU7802_ADCO_B2       0x12
#define NAU7802_ADCO_B3       0x13
#define NAU7802_ADCO_B4       0x14

// NAU7802 Control Bits
#define NAU7802_PU_CTRL_RR    0
#define NAU7802_PU_CTRL_PUD   1
#define NAU7802_PU_CTRL_PUA   2
#define NAU7802_PU_CTRL_PUR   3
#define NAU7802_PU_CTRL_CS    4
#define NAU7802_LDO_3V3       0b100000 // this makes the var = 32


#define DEVICE_ADDR           0x2A

void nau7802_init(void);
void nau7802_reset(void);
void nau7802_power_up(void);
int32_t ISR_ParseNau7802(void);
void nau7802_read_values(uint32_t *value);


#endif /* NAU7802_H_ */
