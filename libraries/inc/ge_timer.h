/**
 * @file  ge_timer.h
 * @brief EE155 library for registering a periodic callback function
 * 
 * Provides an interface for assigning callback functions to the
 * general purpose timers. To use the library, you need to
 * initialize the timer you wish to use and then assign a function
 * to be called when the timer expires.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_TIMER_H
#define _GE_TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

//Includes
#include "common.h"
#include "stm32f30x_tim.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x.h"

//Defines
#define _GE_MAX_TIMERS 16
//minimum time step for timer (1 ms)
#define _GE_TIM_PERIOD 72000
//error code if timer fails to initialize
#define _GE_TIM_ERROR -1

enum _GE_TYPE { GE_SINGLESHOT, GE_PERIODIC };

// Timer ID number returned when registering a callback. Can be
// used to start/stop the appropriate timer

typedef int timer_id_t;


// private helper functions



// public interface functions
int initialize_timer();
int deinit_timer();
timer_id_t register_timer(uint32_t ms, void (*function)(void), uint8_t type);
int start_timer(timer_id_t timer);
int stop_timer(timer_id_t timer);


#ifdef __cplusplus
 }
#endif

#endif