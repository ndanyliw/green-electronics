/*
 * ge_pwm.h
 *
 *  Created on: Oct 19, 2014
 *      Author: Ned
 */

#ifndef GE_PWM_H_
#define GE_PWM_H_

#ifdef __cplusplus
 extern "C" {
#endif

// includes
#include "stm32f30x_tim.h"

//initialize PWM timer (TIMER0)
void init_pwm(void);

//enable pwm channel
void enable_pwm_chan(int chan);

//set pwm count (duty cycle between 0 and 1.0)
void set_pwm(int chan, float duty);

//set pwm frequency
float set_pwm_freq(float freq);

//get the max pwm count as an unnormalized integer
int get_max_pwm_int();

//set the pwm duty cycle as an unnormalized integer
//WARNING, no bounds checking is done
void set_pwm_int(int chan, int compare_val);

#ifdef __cplusplus
 }
#endif

#endif /* GE_PWM_H_ */
