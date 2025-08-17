# STM32F401RE Bare-Metal Programming Projects

Collection of bare-metal embedded C programming projects for the STM32F401RE Nucleo board. All projects built from scratch using only CMSIS standard - no HAL libraries or third-party dependencies. Based on the Udemy course "Embedded Systems Bare-Metal Programming Ground Up™ (STM32)".

## Hardware Used

- **STM32F401RE Nucleo Board**
- ARM Cortex-M4 microcontroller
- Built-in ST-LINK/V2-1 debugger

## Project Categories

### GPIO Projects
- **[gpio_button_input_toggle_led](gpio_button_input_toggle_led/)** - Button input handling with LED toggle
- **[gpio_with_bsrr_toggle_led](gpio_with_bsrr_toggle_led/)** - LED control using BSRR register
- **[toggle_led_setting_addresses](toggle_led_setting_addresses/)** - Direct register address manipulation
- **[toggle_led_setting_addresses_with_structures](toggle_led_setting_addresses_with_structures/)** - Structured register access

### ADC Projects
- **[adc_single_conversion](adc_single_conversion/)** - Single ADC reading implementation
- **[adc_continuous_conversion](adc_continuous_conversion/)** - Continuous ADC sampling

### UART Projects
- **[uart_transfer_to_serial](uart_transfer_to_serial/)** - Basic UART transmission
- **[uart_printing_with_printf](uart_printing_with_printf/)** - Printf implementation over UART
- **[uart_printing_using_separate_c_and_h_files](uart_printing_using_separate_c_and_h_files/)** - Modular UART driver
- **[uart_receiving_rx](uart_receiving_rx/)** - UART reception handling

### Timer Projects
- **[systick_delay](systick_delay/)** - System tick timer for delays
- **[timers_to_toggle_led](timers_to_toggle_led/)** - Timer-based LED control
- **[timer_output_compare_led_toggle](timer_output_compare_led_toggle/)** - Output compare mode
- **[timers_input_capture_frequency](timers_input_capture_frequency/)** - Input capture for frequency measurement

### Interrupt Projects
- **[input_interrupt_to_toggle_led__also_use_of_dwt_to_measure_length_of_execution](input_interrupt_to_toggle_led__also_use_of_dwt_to_measure_length_of_execution/)** - External interrupt with DWT profiling
- **[interrupt_with_adc](interrupt_with_adc/)** - ADC with interrupt handling
- **[interrupt_with_systick](interrupt_with_systick/)** - SysTick interrupt implementation
- **[interrupt_with_timer](interrupt_with_timer/)** - Timer interrupt handling
- **[interrupt_with_uart_serial_monitor_input_rx](interrupt_with_uart_serial_monitor_input_rx/)** - UART RX interrupts
- **[interrupt_with_dma_transfer](interrupt_with_dma_transfer/)** - DMA with interrupt

### I2C Projects
- **[i2c_nau7802_force_sensor](i2c_nau7802_force_sensor/)** - I2C communication with NAU7802 force sensor
- **[i2c_axdl345](i2c_axdl345/)** - I2C communication with ADXL345 accelerometer

## Key Learning Concepts

### Bare-Metal Programming
- Direct register manipulation without HAL abstraction
- Memory-mapped peripheral access
- Understanding ARM Cortex-M4 architecture
- CMSIS standard implementation

### Peripheral Drivers
- GPIO configuration and control
- ADC single and continuous conversion
- UART transmission and reception
- Timer configuration (SysTick, General Purpose)
- External and internal interrupt handling
- I2C master communication
- DMA setup and management

## Development Environment

- **IDE**: STM32CubeIDE
- **Toolchain**: ARM GCC
- **Debugger**: ST-LINK/V2-1
- **Target**: STM32F401RE (ARM Cortex-M4)

## Project Structure

Each project folder contains:
- `Src/main.c` - Main application code
- `Inc/*.h` - Header files for drivers
- `Src/*.c` - Implementation files
- `Startup/startup_stm32f401retx.s` - Startup assembly code
- `STM32F401RETX_FLASH.ld` - Linker script

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/jaredgrinberg/stm32-bare-metal-programming.git
   cd stm32-bare-metal-programming
   ```

2. Open STM32CubeIDE
3. Import existing project from any folder
4. Connect STM32F401RE Nucleo board
5. Build and debug

## Documentation

Reference materials included:
- STM32F401RE Reference Manual
- Cortex-M4 Generic User Guide
- Data sheets and user manuals
