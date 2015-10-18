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
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"
#include "stm32f30x_tim.h"

//macros

#define IS_IC_PIN(PIN)       (((PIN) == PD12) || \
                              ((PIN) == PD13) || \
                              ((PIN) == PD14) || \
                              ((PIN) == PD15) || \
                              ((PIN) == PA11) || \
                              ((PIN) == PA12) || \
                              ((PIN) == PA13) || \
                              ((PIN) == PF6))

//defines
#define GE_IC_IFILTER 0x08

//enumerated types
enum GE_IC_CHAN { IC_CHAN1, IC_CHAN2, IC_CHAN3, IC_CHAN4};

typedef uint8_t ICTimerChan;

void ic_init();
ICTimerChan ic_enable_pin(uint16_t pin, float min_freq);
float ic_read_freq(uint16_t pin);



#ifdef __cplusplus
 }
#endif