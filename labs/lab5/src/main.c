/**
 * @file  main.c
 * @brief Starter code for Lab 5
 * 
 * @details Measures the frequency of the signal on PD12 and prints it
 * to the LCD
 * 
 * @author Ned Danyliw
 * @date  10.2015
 */
#include "ge_libs.h"


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

  //Initialize LCD
  lcd_init();

  //Initialize the USER button as an input
  gpio_setup_pin(DISC_PBTN, GPIO_INPUT, false, false);


  //initialize ic
  ic_init();
  // enable pin PD12 as an input capture with a minimum frequency
  // of 1 Hz
  ic_enable_pin(PD12, 1.0);


  /* Infinite loop */
  /**
   * Flashes the ring of LEDs. If the user button is
   * depressed, it will switch to pulsing the buttons with
   * PWM.
   */
  char buf[10];
  while (1) {
    delay_ms(200);
    lcd_clear();
    sprintf(buf, "%f", ic_read_freq(PD12));
    lcd_goto(0, 0);
    lcd_puts(buf);
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
