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
typedef enum GE_PIN { 
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
} GPIOPin;

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

// Define pin source LUT
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

// Define pin port LUT
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


// Green Electronics Breakout board pin definitions
// Pushbuttons
#define GE_PBTN1        PC0
#define GE_PBTN2        PC1
#define GE_PBTN3        PC2
#define GE_PBTN4        PC3

// LEDs
#define GE_LED1         PD9
#define GE_LED2         PD8
#define GE_LED3         PD11
#define GE_LED4         PD10

// GPIO
#define GE_DIG1         PB8
#define GE_DIG2         PB9
#define GE_DIG3         PB14
#define GE_DIG4         PB15

// PWM
#define GE_PWM1         PA8
#define GE_PWM2         PA9
#define GE_PWM3         PA10

//ADCs (Defined in ge_adc.h)
// // A1 - instrumentation amp (default gain = 5) - PA1
// #define GE_A1           GE_ADC1_2
// // A2 - instrumentation amp (defualt gain = 5) - PA2
// #define GE_A2           GE_ADC1_3
// // A3 - PA3
// #define GE_A3           GE_ADC1_4
// // A4 - PA4
// #define GE_A4           GE_ADC2_1


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