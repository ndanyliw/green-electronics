/**
 * @file  ge_system.c
 * @brief Implementation of system functions for the green
 * electronics libraries.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "ge_system.h"
#include "ge_libs.h"

__IO uint32_t _ge_delay_time = 0;
__IO uint32_t _ge_usb_timeout = 100;


/**
 * @brief Initialize dependancies for libraries
 * @details Initializes the SysTick timer for generating delays.
 * Must be called before using delay_ms.
 */
void ge_init() {
  //Initialize SysTick for delays
  RCC_ClocksTypeDef RCC_clocks;
  RCC_GetClocksFreq(&RCC_clocks);
  SysTick_Config(RCC_clocks.HCLK_Frequency / GE_TICKS_S);

  // initialize GPIO
  gpio_init();

  // initialize ADC library
  adc_init();

  // initialize USART library
  ge_uart_init(115200);



  // initialize timer library
  timer_init();

  // initialize pwm library
  pwm_init();

  // initialize lcd library
  lcd_init();

  // initialize EEPROM
  eeprom_init();

  // initialize ICP library
  ic_int_init();
}


/**
 * @brief Block execution for the specified time in milliseconds
 * @details Sets a counter that is decremented every SysTick
 * and blocks until fully decremented.
 * 
 * @param uint32_t How long in ms to delay.
 */
void delay_ms(__IO uint32_t ms) {
  _ge_delay_time = ms;

  while (_ge_delay_time != 0);
}


/**
 * @brief Interrupt handler for the SysTick
 * @details Interrupt handler for the SysTick
 */
void SysTick_Handler(void) {
  if (_ge_delay_time != 0)
    _ge_delay_time--;

  _ge_usb_timeout--;
}
