/**
 * @file  ge_pins.h
 * 
 * @brief Pin definitions for the STM32 Discovery board used with
 * the EE155 breakout board.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_PINS_H
#define _GE_PINS_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f30x.h"
// #include "common.h"

//number of available pins
#define _GE_PINn 82


/**
 * Enumerate pins so they can be used in lookup tables for
 * GPIO port and bank information.
 */
enum GE_PIN { 
  PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12,
  PA13, PA14, PA15,
  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12,
  PB13, PB14, PB15,
  PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12,
  PC13, PC14, PC15,
  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12,
  PD13, PD14, PD15,
  PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12,
  PE13, PE14, PE15,
  PF0, PF1, PF2, PF4, PF6, PF9, PF10
};

//only define for compilation of library
#ifdef GE_LIB
/** Define appropriate pin number LUT */
static uint16_t _ge_pin_num[] = {
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
  GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
  GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15,
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
  GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
  GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15,
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
  GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
  GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15,
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
  GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
  GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15,
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
  GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
  GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15,
  GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_4,
  GPIO_Pin_6, GPIO_Pin_9, GPIO_Pin_10
};

static uint16_t _ge_pin_source[] = {
  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, 
  GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, 
  GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, 
  GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15,

  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, 
  GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, 
  GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, 
  GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15,

  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, 
  GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, 
  GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, 
  GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15,

  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, 
  GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, 
  GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, 
  GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15,

  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, 
  GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, 
  GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, 
  GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15,

  GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, 
  GPIO_PinSource4, GPIO_PinSource6,
  GPIO_PinSource9, GPIO_PinSource10
};

static GPIO_TypeDef* _ge_pin_port[] = {
  GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA,
  GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA,
  GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB,
  GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB,
  GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC,
  GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC,
  GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD,
  GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD,
  GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE,
  GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE,
  GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF
};
#endif

//Pushbuttons
#define GE_PBTN1        PD11
#define GE_PBTN2        PD10
#define GE_PBTN3        PD13
#define GE_PBTN4        PD12


//ADCs
#define GE_ADC1         PA1
#define GE_ADC2         PA4

//5V tolerant lines (redefine to match silkscreen)
//PA8, PA9, PA10, PB0, PB8, PB9, PC10, PC11
#define PA8_5V          PA8
#define PA9_5V          PA9
#define PA10_5V         PA10
#define PB0_5V          PB0
#define PB8_5V          PB8
#define PB9_5V          PB9
#define PC10_5V         PC10
#define PC11_5V         PC11

//Discovery board specific pins
//USER pushbutton
#define DISC_PBTN       PA0

//LEDs
#define DISC_LD3        PE9
#define DISC_LD4        PE8
#define DISC_LD5        PE10
#define DISC_LD6        PE15
#define DISC_LD7        PE11
#define DISC_LD8        PE14
#define DISC_LD9        PE12
#define DISC_LD10       PE13





#ifdef __cplusplus
 }
#endif

#endif