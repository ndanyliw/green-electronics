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
#include "common.h"
#include "stm32f30x_tim.h"

#define PWM_CHAN1 1
#define PWM_CHAN2 2
#define PWM_CHAN3 3
#define PWM_CHAN4 4

//initialize PWM timer (TIMER0)
void pwm_init(void);

//enable pwm channel
void pwm_enable_chan(int chan);

//set pwm count (duty cycle between 0 and 1.0)
void pwm_set(int chan, float duty);

//set pwm frequency
float pwm_freq(float freq);

//get the max pwm count as an unnormalized integer
int pwm_get_max_int();

//set the pwm duty cycle as an unnormalized integer
//WARNING, no bounds checking is done
void pwm_set_int(int chan, int compare_val);

#ifdef __cplusplus
 }
#endif

#endif /* GE_PWM_H_ */
