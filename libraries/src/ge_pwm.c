/**
 * @file ge_pwm.c
 * @brief PWM library
 * 
 * @author Ned Danyliw
 * @date 09.2016
 */

#include "ge_pwm.h"

// how many ticks to make PWM period
int _ge_pwm_period;

/**
 * @brief Initialize PWM library
 * @details Initializes the PWM library. This includes setting
 * up TIM1 as the PWM timer and enabling the output compare
 * channels on TIM1.
 */
void pwm_init(void) {
  // initialize PWM period
  _ge_pwm_period = 65535;

  TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
  TIM_OCInitTypeDef TIM_OC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // Setup TIM1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBase_InitStructure);
  TIM_OCStructInit(&TIM_OC_InitStructure);

  TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBase_InitStructure.TIM_Period = _ge_pwm_period;
  TIM_TimeBase_InitStructure.TIM_Prescaler = 0; // 72 Mhz
  TIM_TimeBase_InitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBase_InitStructure);

  // Enable OC channels
  TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OC_InitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC_InitStructure.TIM_Pulse = 65536/2 - 1;

  // enable channels 1, 2, 3, and 4
  TIM_OC1Init(TIM1, &TIM_OC_InitStructure);
  TIM_OC1PreloadConfig (TIM1, TIM_OCPreload_Enable);

  TIM_OC2Init(TIM1, &TIM_OC_InitStructure);
  TIM_OC2PreloadConfig (TIM1, TIM_OCPreload_Enable);

  TIM_OC3Init(TIM1, &TIM_OC_InitStructure);
  TIM_OC3PreloadConfig (TIM1, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM1, &TIM_OC_InitStructure);
  TIM_OC4PreloadConfig (TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, DISABLE);
  TIM_Cmd(TIM1, ENABLE);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**
 * @brief Sets the pin to connect to the appropriate timer channel
 * @details Looks through a lookup table to correctly initialize the
 * specified pin and connect to the associate timer channel.
 * 
 * Available pins are: PE9, PE11, PE13, PE14, PA8, PA9, PA10, and PA11
 * 
 * @param pin Pin name to connect to. @ref ge_pins.h
 * @return The associated timer channel for the pin. Or PWM_PIN_INVALID
 * if the pin is not a valid PWM pin.
 */
int pwm_set_pin(int pin) {
  //check if valid pin
  if (!IS_PWM_PIN(pin))
    return PWM_PIN_INVALID;

  //select appropriate alternate function
  uint8_t pin_af;
  int chan;
  switch (pin) {
    case PA8:
      pin_af = GPIO_AF_6;
      chan = PWM_CHAN1;
      break;
    case PA9:
      pin_af = GPIO_AF_6;
      chan = PWM_CHAN2;
      break;
    case PA10:
      pin_af = GPIO_AF_6;
      chan = PWM_CHAN3;
      break;
    case PA11:
      pin_af = GPIO_AF_11;
      chan = PWM_CHAN4;
      break;
    case PE9:
      pin_af = GPIO_AF_2;
      chan = PWM_CHAN1;
      break;
    case PE11:
      pin_af = GPIO_AF_2;
      chan = PWM_CHAN2;
      break;
    case PE13:
      pin_af = GPIO_AF_2;
      chan = PWM_CHAN3;
      break;
    case PE14:
      pin_af = GPIO_AF_2;
      chan = PWM_CHAN4;
      break;  
  }

  //Initialize associate GPIO
  GPIO_InitTypeDef pwm_pin_struct;
  GPIO_StructInit(&pwm_pin_struct);

  pwm_pin_struct.GPIO_Pin = _ge_pin_num[pin];
  pwm_pin_struct.GPIO_OType = GPIO_OType_PP;
  pwm_pin_struct.GPIO_PuPd = GPIO_PuPd_UP;
  pwm_pin_struct.GPIO_Speed = GPIO_Speed_50MHz;
  pwm_pin_struct.GPIO_Mode = GPIO_Mode_AF;

  GPIO_Init(_ge_pin_port[pin], &pwm_pin_struct);
  //set alternate function
  GPIO_PinAFConfig(_ge_pin_port[pin], _ge_pin_source[pin], pin_af);

  //return the connected channel
  return chan;
}


/**
 * @brief Set the PWM value of the specified channel
 * @details Sets the PWM value of the specified channel as a
 * float. The method handles converting this to a number of
 * timer ticks that is used by the actual output compare
 * registers.
 * 
 * @param chan Channel to set (between 1-4)
 * @param duty Duty cycle for PWM pin (between 0-1.0)
 */
void pwm_set(int chan, float duty) {
  int compare_val = (int)(duty*(float)_ge_pwm_period);
  pwm_set_int(chan,compare_val);
}


/**
 * @brief Return the maximum integer that can be used for an
 * output compare register.
 * @details Returns the max period of the PWM timer
 * @return Max integer for output compare
 */
int pwm_get_max_int()
{
	return _ge_pwm_period;
}


/**
 * @brief Set the output compare register directly
 * @details Sets the ouptut compare register directly. No bounds
 * checking is done so the programmer has to know what the
 * maximum allowed value is.
 * 
 * @param chan Channel to set (between 1-4)
 * @param compare_val Integer value to set register
 */
void pwm_set_int(int chan, int compare_val)
{
	 switch(chan) {
	    case 1:
	      TIM_SetCompare1(TIM1, compare_val);
	      break;
	    case 2:
	      TIM_SetCompare2(TIM1, compare_val);
	      break;
	    case 3:
	      TIM_SetCompare3(TIM1, compare_val);
	      break;
	    case 4:
	      TIM_SetCompare4(TIM1, compare_val);
	      break;
	    default:
	      //do nothing
	      break;
	  }
}


/**
 * @brief Sets PWM frequency
 * @details Sets the PWM frequency by setting up TIM1 appropriately
 * and setting the PWM period. The method then returns the actual
 * frequency of the PWM signal.
 * 
 * @param freq Frequency to set in Hz as a float
 * @return The actual achieved frequency
 */
float pwm_freq(float freq) {
  float base_freq = 72000000.0; //arm clock frequency
  float actual_freq = 0.0;

  //calculate period relative to master clock
  float master_period = base_freq/freq;

  int prescaler = 0;

  // handle high frequency cases where we must shorten our period
  if (master_period <= 65536) {
    _ge_pwm_period = (int)master_period - 1;
    actual_freq = base_freq/(float)_ge_pwm_period;
  } else {
    prescaler = (int)((master_period/65536.0));
    float new_freq = base_freq/((float)(prescaler + 1));
    _ge_pwm_period = (int)(new_freq/freq) - 1;
    actual_freq = new_freq/(float)_ge_pwm_period;
  }

  TIM_PrescalerConfig(TIM1, prescaler, TIM_PSCReloadMode_Update);
  TIM_SetAutoreload(TIM1, _ge_pwm_period);

  return actual_freq;
}
