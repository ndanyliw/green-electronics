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
#define _GE_TIM_PERIOD 720000
//error code if timer fails to initialize
#define _GE_TIM_ERROR -1

enum _GE_TYPE { GE_SINGLESHOT, GE_PERIODIC };

// Timer ID number returned when registering a callback. Can be
// used to start/stop the appropriate timer

typedef int timer_id_t;


// private helper functions



// public interface functions
int timer_init();
int timer_deinit();
timer_id_t timer_register(uint32_t ms, void (*function)(void), uint8_t type);
int timer_start(timer_id_t timer);
int timer_stop(timer_id_t timer);
void timer_set_period(uint32_t counts);
uint32_t timer_get_period();


#ifdef __cplusplus
 }
#endif

#endif