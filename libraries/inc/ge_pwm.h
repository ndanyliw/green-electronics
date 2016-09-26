/**
 * @file ge_pwm.h
 * @brief PWM library
 * 
 * @author Ned Danyliw
 * @date 09.2016
 */

#ifndef GE_PWM_H_
#define GE_PWM_H_

#ifdef __cplusplus
 extern "C" {
#endif

// includes
#include "common.h"
#include "stm32f30x_tim.h"
#include "ge_pins.h"

// macros
#define IS_PWM_PIN(PIN)            (((PIN) == PA8) || \
                                    ((PIN) == PA9) || \
                                    ((PIN) == PA10) || \
                                    ((PIN) == PA11) || \
                                    ((PIN) == PE9) || \
                                    ((PIN) == PE11) || \
                                    ((PIN) == PE13) || \
                                    ((PIN) == PE14))  


//defines
#define PWM_CHAN1 1
#define PWM_CHAN2 2
#define PWM_CHAN3 3
#define PWM_CHAN4 4

#define PWM_PIN_INVALID -1

//initialize PWM timer (TIMER1)
void pwm_init(void);

//connects specified pin to the appropriate timer channel.
//look at the source code to see available pins
int pwm_set_pin(int pin);

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
