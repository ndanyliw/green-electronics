/**
 * @file  main.c
 * @brief Project template the Green Electronics libraries.
 * 
 * @details Empty template 
 * 
 * @author Ned Danyliw
 * @date  09.2016
 */
#include "ge_libs.h"
#include <stdio.h>


///////////////////////////////////////
// Initializations 
///////////////////////////////////////
//
// Write global variable initializations and define statements here



///////////////////////////////////////
// Functions 
///////////////////////////////////////
//
// Helper functions here



/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // Initialize library
  ge_init();

  // Initialize the USER button as an input
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);

  // Initialize PBTN1
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);

  // Print Hello World
  lcd_clear();
  lcd_goto(0, 0);
  lcd_puts("Hello, World!");

  // Setup timer library
  // Set minimum timestep to 1ms (number of counts referecned to 
  // a 72MHz clock)
  timer_set_timestep(72000);


  /* Infinite loop */
  /**
   * Handles the user interface state machine
   * Your logic goes here!
   */
  while (1) {   



    delay_ms(50);
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
