/**
 * @file  ge_gpio.h
 * @brief Library of helper functions for interfacing with the GPIO
 * on the STM32 discovery board.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */


#ifndef _GE_GPIO_H
#define _GE_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "common.h"
#include "ge_pins.h"

typedef uint8_t GPIOPin;

//Defines
//GPIO directions
enum GPIO_DIR { GPIO_INPUT, GPIO_OUTPUT };
//GPIO states
enum GPIO_STATE { GPIO_LOW, GPIO_HIGH };



//methods
void gpio_init();
void gpio_deinit();
void gpio_setup_pin(GPIOPin pin, int dir, bool open_drain, bool pull_up);
void gpio_write_pin(GPIOPin pin, uint8_t state);
uint8_t gpio_read_pin(GPIOPin pin);


#ifdef __cplusplus
 }
#endif

#endif
