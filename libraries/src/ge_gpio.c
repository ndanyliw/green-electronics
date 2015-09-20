/**
 * @file  ge_gpio.c
 * @brief Implementation of GPIO library for the STM32 discovery
 * board.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "ge_gpio.h"


/**
 * @brief Initializes the Discovery board's GPIO
 * @details Initializes all of the STM32 GPIO banks. The
 * method doesn't set them to any particular mode or value.
 * It just makes sure the peripherals are enabled and can
 * be set by later functions.
 */
void gpio_init() {
  //enable clocks for GPIO banks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
}


/**
 * @brief Deinitialize GPIO clocks
 * @details Turns off the clocks to the GPIO banks. This
 * function should only be called if you need to fully disable
 * the GPIO banks.
 */
void gpio_deinit() {
  //disable clocks for GPIO banks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, DISABLE);
}


/**
 * @brief Sets whether a pin is an input or output
 * @details Sets up a pin either as an input or output. This will
 * overwrite any previous pin settings. This method also can set
 * whether to use an internal pull-up resistor and if the pin
 * is set as an output whether to be an open drain or push pull
 * driven pin.
 * 
 * @param pin Specified pin name (i.e. PD1, PA12, etc.)
 * @param dir GPIO_INPUT or GPIO_OUTPUT
 * @param open_drain True sets output to open drain. False to push-pull
 * @param pull_up True enables an internal pull up resistor.
 */
void gpio_setup_pin(GPIOPin pin, int dir, bool open_drain, bool pull_up) {
  //create initialization structure
  GPIO_InitTypeDef GPIO_init;
  GPIO_StructInit(&GPIO_init);

  GPIO_init.GPIO_Pin = _ge_pin_num[pin];
  //where the magic happens
  if (dir == GPIO_INPUT) {
    GPIO_init.GPIO_Mode = GPIO_Mode_IN;
  } else {
    GPIO_init.GPIO_Mode = GPIO_Mode_OUT;
    
    //handle output type
    if (open_drain) {
      GPIO_init.GPIO_OType = GPIO_OType_OD;
    } else {
      GPIO_init.GPIO_OType = GPIO_OType_PP;
    }
    
    GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
  }

  //handle optional internal pull up
  if (pull_up) {
    GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
  } else {
    GPIO_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  }

  GPIO_Init(_ge_pin_port[pin], &GPIO_init);
}


/**
 * @brief Sets the specified pin either high or low
 * @details Sets the specified pin either high or low
 * 
 * @param pin Pin to write to
 * @param state GPIO_HIGH (1) or GPIO_LOW (0)
 */
void gpio_write_pin(GPIOPin pin, uint8_t state) {
  GPIO_WriteBit(_ge_pin_port[pin], _ge_pin_num[pin], state);
}


/**
 * @brief Read data bit on specified GPIO pin
 * @details Read data bit on specified GPIO pin
 * 
 * @param pin Pin to read from
 * @return Bit value of pin
 */
uint8_t gpio_read_pin(GPIOPin pin) {
  return GPIO_ReadInputDataBit(_ge_pin_port[pin], _ge_pin_num[pin]);
}

