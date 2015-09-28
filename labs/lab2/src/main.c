/**
 * @file  main.c
 * @brief Main program loop for Lab 2
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "lab2.h"

// Preprocessor trick:
// This converts a number to a string at compile-time so that a #define'd number
// can be pasted into a string.
// The two macros are required to keep the preprocessor from using the name
// passed to STR(x) instead of its value.
//
// Thanks to http://stackoverflow.com/questions/1595544/c-macro-turn-a-number-into-a-string
#define VAL(x) #x
#define STR(x) VAL(x)

//Define operating states
enum DISP_STATES {DISP_MAIN, DISP_OFF, DISP_CALV, DISP_CALI};

uint8_t state = DISP_MAIN;
bool btn_pressed = false;

void change_state() {
  if (gpio_read_pin(GE_PBTN1)) {
    if (!btn_pressed) {
      state++;
      if (state > DISP_CALI) state = DISP_MAIN;
    }
    btn_pressed = true;
  } else {
    btn_pressed = false;
  }
}


/**
 * @brief Initializes buttons
 * @details Sets PB1 to go to the next menu screen.
 * PB2 to trigger calibration.
 */
void setup_buttons() {
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);
}

void start_conversion() {
  adc_set_fs(1000);
  adc_enable_channel(1);

  adc_callback(1, &my_adc_callback);
  adc_start();
}

int main() {
  //Initialize systems
  ge_init();

  lcd_init();
  adc_init();

  gpio_init();

  setup_buttons();

  timer_init();

  timer_id_t state_tim = timer_register(50, &change_state, GE_PERIODIC);

  timer_start(state_tim);

  //initialize power meter
  meter_init();

  //handle display
  uint8_t last_state = 255;
  while (1) {

    if (state != last_state) {
      lcd_clear();
      last_state = state;
    }

    switch(state) {
      case DISP_MAIN:
        meter_display();
        break;
      case DISP_OFF:
        lcd_goto(0, 0);
        lcd_puts("Calibration: offsets");
        lcd_goto(0, 1);
        lcd_puts("Apply 0V and 0A");
        lcd_goto(0, 3);
        lcd_puts("2 - OK");

        if (gpio_read_pin(GE_PBTN2)) {
          calibrate_offset();
          lcd_goto(0, 2);
          lcd_puts("Stored");
        }
        break;
      case DISP_CALV:
        lcd_goto(0, 0);
        lcd_puts("Calibration: voltage");
        lcd_goto(0, 1);
        lcd_puts("Apply " STR(CAL_VOLTS) "V");
        lcd_goto(0, 3);
        lcd_puts("2 - OK");

        if (gpio_read_pin(GE_PBTN2)) {
          calibrate_voltage();
          lcd_goto(0, 2);
          lcd_puts("Stored");
        }
        break;
      case DISP_CALI:
        lcd_goto(0, 0);
        lcd_puts("Calibration: current");
        lcd_goto(0, 1);
        lcd_puts("Apply " STR(CAL_CURR) "A");
        lcd_goto(0, 3);
        lcd_puts("2 - OK");

        if (gpio_read_pin(GE_PBTN2)) {
          calibrate_current();
          lcd_goto(0, 2);
          lcd_puts("Stored");
        }
        break;
      default:
        state = DISP_MAIN;
        break;
    }

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