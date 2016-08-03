/**
 * @file  ge_ic.h
 * @brief Library for using the input capture registers on the timers
 * and calculate the read frequency. This library uses Timer 4 for the
 * input capture.
 * 
 * @author Ned Danyliw
 * @date  10.2015
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

#define IS_IC_PIN(PIN)       ((PIN) == PD12)



void ic_init();
float ic_read_freq();



#ifdef __cplusplus
 }
#endif

#endif