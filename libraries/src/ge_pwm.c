/*
 * ee152_pwm.c
 *
 *  Created on: Oct 19, 2014
 *      Author: Ned
 */

#include "ge_pwm.h"

int _ge_pwm_period;

//initialize PWM timer (TIMER0)
void pwm_init(void) {
  _ge_pwm_period = 65535;

  TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
  TIM_OCInitTypeDef TIM_OC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBase_InitStructure);
  TIM_OCStructInit(&TIM_OC_InitStructure);

  TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBase_InitStructure.TIM_Period = _ge_pwm_period;
  TIM_TimeBase_InitStructure.TIM_Prescaler = 0; // 72 Mhz
  TIM_TimeBase_InitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBase_InitStructure);

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

//  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

  TIM_ARRPreloadConfig(TIM1, DISABLE);
  TIM_Cmd(TIM1, ENABLE);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);

//  TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
}

/* DEPRECATED */
//enable pwm channel
void pwm_enable_chan(int chan) {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = (0x1)<<(chan-1 + 8);


  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  switch(chan) {
    case PWM_CHAN1:
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
      break;
    case PWM_CHAN2:
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
      break;
    case PWM_CHAN3:
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_6);
      break;
    case PWM_CHAN4:
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_11);
      break;
  }
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

//set pwm count (16 bit unsigned)
void pwm_set(int chan, float duty) {
  int compare_val = (int)(duty*(float)_ge_pwm_period);
  pwm_set_int(chan,compare_val);
}

int pwm_get_max_int()
{
	return _ge_pwm_period;
}

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

//set pwm frequency (returns the actual frequency the pwm wave is set to)
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
