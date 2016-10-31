/**
 * @file  ge_ic.h
 * @brief Library for using the input capture registers on the timers
 * and calculate the read frequency. This library uses Timer 4 for the
 * input capture. The ICP pin is set to be PB10
 * 
 * @author Ned Danyliw
 * @date  9.2016
 */


#ifndef _GE_IC_H
#define _GE_IC_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"
#include "stm32f30x_tim.h"

#include "ge_gpio.h"
#include "stm32f30x_exti.h"

//macros

#define IS_IC_INT_PIN(PIN)       ((PIN) == PB10)



void ic_int_init();
float ic_int_read_freq();
void ic_int_set_maxf(float f);



#ifdef __cplusplus
 }
#endif

#endif