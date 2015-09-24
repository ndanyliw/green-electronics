/**
 * @file  main.c
 * @brief Hello World demo for the Green Electronics libraries.
 * 
 * @details Prints "Hello World" on the LCD and blinks the onboard
 * LEDs
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */
#include "ge_libs.h"


void setup_led_gpio() {
  //Initialize LED pins and set as outputs
  gpio_setup_pin(DISC_LD3, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD4, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD5, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD6, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD7, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD8, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD9, GPIO_OUTPUT, false, false);
  gpio_setup_pin(DISC_LD10, GPIO_OUTPUT, false, false);
}

void led_off() {
  gpio_write_pin(DISC_LD3, GPIO_LOW);
  gpio_write_pin(DISC_LD4, GPIO_LOW);
  gpio_write_pin(DISC_LD5, GPIO_LOW);
  gpio_write_pin(DISC_LD6, GPIO_LOW);
  gpio_write_pin(DISC_LD7, GPIO_LOW);
  gpio_write_pin(DISC_LD8, GPIO_LOW);
  gpio_write_pin(DISC_LD9, GPIO_LOW);
  gpio_write_pin(DISC_LD10, GPIO_LOW);
}

void led_on() {
  gpio_write_pin(DISC_LD3, GPIO_HIGH);
  gpio_write_pin(DISC_LD4, GPIO_HIGH);
  gpio_write_pin(DISC_LD5, GPIO_HIGH);
  gpio_write_pin(DISC_LD6, GPIO_HIGH);
  gpio_write_pin(DISC_LD7, GPIO_HIGH);
  gpio_write_pin(DISC_LD8, GPIO_HIGH);
  gpio_write_pin(DISC_LD9, GPIO_HIGH);
  gpio_write_pin(DISC_LD10, GPIO_HIGH);
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
