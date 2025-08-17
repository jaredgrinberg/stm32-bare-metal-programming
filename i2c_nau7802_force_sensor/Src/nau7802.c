/*
 * nau7802.c
 *
 *  Created on: May 27, 2024
 *      Author: jared
 */

#include "nau7802.h"
#include "uart.h"

char data;
uint8_t data_rec[3];

void nau7802_reset(void) {
    // Reset the NAU7802
    data = 1 << NAU7802_PU_CTRL_RR;
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_PU_CTRL, 1, &data);

    // Wait for reset to complete
    data = 0 << NAU7802_PU_CTRL_RR;
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_PU_CTRL, 1, &data);
}

void nau7802_power_up(void) {
    // Power up sequence
    data = 1 << NAU7802_PU_CTRL_PUD;
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_PU_CTRL, 1, &data);

    // Wait for Power Up Digital
    do {
        I2C1_byteRead(DEVICE_ADDR, NAU7802_PU_CTRL, (char *)&data);
    } while (!(data & (1 << NAU7802_PU_CTRL_PUD)));

    // Enable Power Up Analog
    data |= 1 << NAU7802_PU_CTRL_PUA;
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_PU_CTRL, 1, &data);

    // Wait for Power Up Analog
    do {
        I2C1_byteRead(DEVICE_ADDR, NAU7802_PU_CTRL, (char *)&data);
    } while (!(data & (1 << NAU7802_PU_CTRL_PUA)));
}

void nau7802_init(void) {
    uart2_tx_init();
    UART_Println("Initializing NAU7802...");

    I2C1_init();

    // Reset the NAU7802
    // Case 0 and 1
    nau7802_reset();
    UART_Println("NAU7802 Reset");

    // Power up the NAU7802
    // Case 2, 3, 4, and 5
    nau7802_power_up();
    UART_Println("NAU7802 Powered Up");

    // Set LDO voltage to 3.3V
    // Case 6
    data = NAU7802_LDO_3V3;
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL1, 1, &data);
    UART_Println("LDO voltage set to 3.3V");

    // Wait for the LDO voltage to be set
	// Case 7
    char val;
    do {
        I2C1_byteRead(DEVICE_ADDR, NAU7802_CTRL1, &val);
        UART_Print("Waiting for LDO voltage to stabilize: ");
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "val = %d\r\n", val);
        UART_Print(buffer);
    } while (val < 32);

	// Select AVDD source to internal LDO
	// Case 8
	data = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 7);
	I2C1_burstWrite(DEVICE_ADDR, NAU7802_PU_CTRL, 1, &data);
	UART_Println("Selected AVDD source to internal LDO");

	// Wait for the internal LDO to stabilize
	// Case 9
	do {
		I2C1_byteRead(DEVICE_ADDR, NAU7802_PU_CTRL, &val);
		UART_Print("Waiting for internal LDO to stabilize: ");
		UART_Println((val >= 142) ? "OK" : "Not yet");
	} while (val < 142);

    // Set gain to 128
    // Case 10
    data = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5);
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL1, 1, &data);
    UART_Println("Gain set to 128x");

    // Ensure the gain is set
    // Case 11
    do {
        I2C1_byteRead(DEVICE_ADDR, NAU7802_CTRL1, &val);
    } while (val < 39);

    // Set sampling rate to 300 SPS
    // Case 12
    data = (1 << 4) | (1 << 5);
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL2, 1, &data);
    UART_Println("Sampling rate set to 300 SPS");

    // Set clock frequency
    // Case 13
    data = (1 << 4) | (1 << 5);
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL_ADC, 1, &data);

    // Enable PGA output bypass capacitor
    // Case 14
    data = (1 << 7);
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL_PWR, 1, &data);

    // Trigger calibration sequence
    // Case 15
    data = (1 << 2) | (1 << 4) | (1 << 5);
    I2C1_burstWrite(DEVICE_ADDR, NAU7802_CTRL2, 1, &data);
    UART_Println("Calibration started");

    // Wait for calibration to complete
    // Case 16
    uint32_t prev2 = 0; // Replace with appropriate timing function if available
    uint32_t now2;
    do {
        now2 = prev2 + 1000000; // Simulate a 1ms delay
        I2C1_byteRead(DEVICE_ADDR, NAU7802_CTRL2, &val);
    } while ((now2 - prev2 < 1000) && (val < 48));
    UART_Println("Calibration completed");
}

int32_t ISR_ParseNau7802(void) {
    int32_t reading = ((data_rec[0] << 16) | (data_rec[1] << 8) | data_rec[2]) & 0xffffff;

    // Perform sign extension if the result is negative
    if (reading & 0x00800000) {
        reading |= 0xff000000;
    }

    // Scale the reading and shift to make 0 the center of uint16 range
    int32_t scaled_reading = reading / 256 + 32768;

    // Clamp the scaled reading to be within uint16_t range
    if (scaled_reading < 0) {
        scaled_reading = 0;
    } else if (scaled_reading > 65535) {
        scaled_reading = 65535;
    }

    return (uint16_t)scaled_reading;
}

void nau7802_read_values(uint32_t *value) {
    // Read ADC output values
    I2C1_burstRead(DEVICE_ADDR, NAU7802_ADCO_B2, 3, (char *)data_rec);
    *value = ISR_ParseNau7802();
}
