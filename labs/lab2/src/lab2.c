/**
 * @file  lab2.c
 * @brief Starter code for lab 2.
 * 
 * @details Modify this file to implement the power meter
 * for lab 2
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "lab2.h"


/**
 * @brief Calibrate out the 0V and 0A offsets
 * @details Reads the ADC value at 0V and 0A and records
 * their offsets in the EEPROM
 */
void calibrate_offset() {

}


/**
 * @brief Updates calibration for the standard voltage
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_voltage() {

}


/**
 * @brief Updates calibration for the standard current
 * @details Calculates the calibration value read from the ADC
 * and stores the result in the EEPROM
 */
void calibrate_current() {

}


/**
 * @brief Initialize the energy meter
 * @details Reads the calibration values from the EEPROM
 */
void meter_init() {

}


/**
 * @brief Displays energy meter data
 * @details Replace with code to update the display with
 * your own
 */
void meter_display() {
  //Code here
}


/**
 * @brief Callback at end of ADC conversion
 * @details Called at the end of the ADC conversions
 */
void my_adc_callback(uint32_t data) {
  uint16_t voltage = (uint16_t) (data & 0x0000ffff);
  uint16_t current = (uint16_t) (data >> 16);

  
}





/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  //Initialize library
  ge_init();

  //Initialize GPIO
  gpio_init();

  setup_led_gpio();

  //Initialize the USER button as an input
  gpio_setup_pin(DISC_PBTN, GPIO_INPUT, false, false);

  //Initialize LCD
  lcd_init();

  // //Print Hello World
  lcd_clear();
  lcd_goto(0, 0);
  lcd_puts("Hello, World!");

  // timer_init();

  //Initialize VCOM
  // vcom_init();
  // vcom_send("Hello, World!\n");

  /* Infinite loop */
  /**
   * Flashes the ring of LEDs. If the user button is
   * depressed, it will switch to pulsing the buttons with
   * PWM.
   */
  while (1) {   
    //check if button depressed
    if (!gpio_read_pin(DISC_PBTN)) {
      /* LEDs Off */
      led_off();
      delay_ms(500); /*500ms - half second*/
      
      /* LEDs Off */
      led_on();
      delay_ms(500); /*500ms - half second*/

      // vcom_send("Hi\n");
    } else {
      /* LEDs Off */
      led_off();
      delay_ms(100); /*500ms - half second*/
      
      /* LEDs Off */
      led_on();
      delay_ms(100); /*500ms - half second*/
    }
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
