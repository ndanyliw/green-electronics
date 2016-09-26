/**
 * @file  main.c
 * @brief Hello World demo for the Green Electronics libraries.
 * 
 * @details Demos the basic functionality of the Green Electronics
 * libraries.
 * 
 * @author Ned Danyliw
 * @date  09.2016
 */
#include "ge_libs.h"
#include <stdio.h>

__IO bool led_state;

bool led_speed;
timer_id_t led_timer;

// keep track of what state the board is in
uint8_t ui_state;

#define NUM_STATES 4
// enumerate states
enum UI_STATE {
  LED_DEMO, PWM_DEMO, ADC_DEMO, USART_DEMO
};

// ADC variables
#define NUM_ADC 4
__IO uint16_t val[NUM_ADC];
ADC_CHAN_Type chan_to_conv[NUM_ADC] = {
  GE_ADC1_2, GE_ADC1_3, GE_ADC1_4, GE_ADC2_1
};
int num_refresh;

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

// method to toggle LEDs
void toggle_led() {
  if (led_state) {
    led_off();
    led_state = false;
  } else {
    led_on();
    led_state = true;
  }
}

// handle initilization code for changing states
void change_state() {
  switch (ui_state) {
    case LED_DEMO:
      setup_led_gpio();
      led_timer = timer_register(500, &toggle_led, GE_PERIODIC);
      timer_start(led_timer);
      lcd_clear();
      lcd_goto(0, 0);
      lcd_puts("LED Demo");

      printf("LED Demo\n");
      break;
    case PWM_DEMO:
      // enable pwm pins
      pwm_set_pin(PE9);
      pwm_set_pin(PE11);
      pwm_set_pin(PE13);
      pwm_set_pin(PE14);

      lcd_clear();
      lcd_goto(0, 0);
      lcd_puts("PWM Demo");

      printf("PWM Demo\n");
      break;
    case ADC_DEMO:
      lcd_clear();
      lcd_goto(0, 0);
      lcd_puts("ADC Demo");

      printf("ADC Demo\n");
      break;
    case USART_DEMO:
      lcd_clear();
      lcd_goto(0, 0);
      lcd_puts("USART Demo");
      lcd_goto(0, 1);

      printf("USART Demo. Echo user input.\n");
      break;
    default:
      break;
  }
}

// ADC callback
void my_adc_callback(uint16_t *buf) {
  for (int k = 0; k < NUM_ADC; k++) {
    val[k] = buf[k];
  }
}


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

  // Initialize LEDs
  setup_led_gpio();
  led_state = false;
  led_speed = false;

  // Initialize the USER button as an input
  gpio_setup_pin(GE_PBTN2, GPIO_INPUT, false, false);

  // Initialize PBTN1
  gpio_setup_pin(GE_PBTN1, GPIO_INPUT, false, false);

  // Print to serial port
  printf("Hello, World!\n");

  // Print Hello World
  lcd_clear();
  lcd_goto(0, 0);
  lcd_puts("Hello, World!");

  // Setup timer library
  // Set minimum timestep to 1ms (number of counts referecned to 
  // a 72MHz clock)
  timer_set_timestep(72000);
  // register callback for toggling LEDs every 500ms
  led_timer = timer_register(500, &toggle_led, GE_PERIODIC);
  timer_start(led_timer);

  // set mode to the LED demo
  ui_state = LED_DEMO;

  // set pwm level for PWM demo
  float pwm_level = 0.0;

  // setup PWM library
  pwm_freq(10000.0);

  // setup ADC
  // set sampling rate to 10kHz
  adc_set_fs(10000);
  // register ADC callback
  adc_callback(&my_adc_callback);
  // enable ADC channels
  adc_enable_channels(chan_to_conv, NUM_ADC);
  adc_initialize_channels();
  adc_start();

  // keep track of how many times the UI has looped
  num_refresh = 0;


  /* Infinite loop */
  /**
   * Handles the user interface state machine
   */
  while (1) {   
    switch (ui_state) {
      /**
       * User can toggle the flashing speed of the Discovery board
       * LEDs. This demos how the timer library can be used.
       */
      case LED_DEMO:
        //check if button depressed
        if (!gpio_read_pin(GE_PBTN2)) {
          if (led_speed) {
            timer_set_period(led_timer, 500);
            led_speed = false;
          } else {
            timer_set_period(led_timer, 100);
            led_speed = true;
          }

          // wait for button to be released
          while (!gpio_read_pin(GE_PBTN2));
        }
        break;

      case PWM_DEMO:
        pwm_level = pwm_level + .05;

        if (pwm_level > 1.0) pwm_level = 0.0;

        pwm_set(PWM_CHAN1, pwm_level);
        pwm_set(PWM_CHAN2, pwm_level);
        pwm_set(PWM_CHAN3, pwm_level);
        pwm_set(PWM_CHAN4, pwm_level);
        break;

      case ADC_DEMO:
        // print results every 10 refreshes
        num_refresh++;
        if (num_refresh >= 10) {
          num_refresh = 0;
          printf("%u\t%u\t%u\t%u\n", val[0], val[1], val[2], val[3]);
          // printf("%u\t%u\n", val[0], val[1]);
        }
        break;

      case USART_DEMO: ;
        if (ge_uart_available()) {
          uint8_t c = ge_uart_get();
          printf("%c\n", c);
          lcd_putc(c);
        }
        break;

      default:
        break;
    }

    // check whether to change state
    if (!gpio_read_pin(GE_PBTN1)) {
      // stop LED timer if necessary
      if (ui_state == LED_DEMO) timer_stop(led_timer);

      ui_state++;
      if (ui_state >= NUM_STATES) ui_state = LED_DEMO;

      change_state();

      // wait for button to be released
      while (!gpio_read_pin(GE_PBTN1));
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
