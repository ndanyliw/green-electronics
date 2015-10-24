/**
 * @file  ge_ic.c
 * @brief Implementation of input capture library
 * 
 * @author Ned Danyliw
 * @date  10.2015
 */

#include "ge_ic.h"
#include "ge_pins.h"


//private methods
void _ic_enable_chan(uint8_t chan);
ICTimerChan _ic_get_chan(uint16_t pin);

//private variables
//prescaler for timer
static uint16_t _ge_ic_prescaler = 0;
//most recent captured values
static __IO uint16_t _ge_ic_chan_captures[4];
static __IO uint16_t _ge_ic_chan_captures_last[4];


/**
 * @brief Initialize input capture timer
 * @details Initializes TIM4 for the input capture library.
 */
void ic_init() {
  //enable clocks for TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


  TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
  TIM_ICInitTypeDef TIM_IC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //setup TIM4
  TIM_TimeBaseStructInit(&TIM_TimeBase_InitStructure);
  TIM_ICStructInit(&TIM_IC_InitStructure);

  TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBase_InitStructure.TIM_Period = 0xffff;
  TIM_TimeBase_InitStructure.TIM_Prescaler = _ge_ic_prescaler; // 72 Mhz
  TIM_TimeBase_InitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBase_InitStructure);

  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  //setup NVIC
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  //enable interrupts
  TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  //enable timer
  TIM_Cmd(TIM4, ENABLE);
}


/**
 * @brief Enable a pin as an input capture pin
 * @details Enables the appropriate timer channel and GPIO setting it
 * as an input capture input. Note that this could possibly overwrite
 * timer settings if multiple used pins share the same timer channel.
 * 
 * @param pin Pin to use for IC
 * @return The associated IC timer channel
 */
ICTimerChan ic_enable_pin(uint16_t pin, float min_freq) {
  assert_param(IS_IC1_PIN(pin) || IS_IC4_PIN(pin));

  ICTimerChan chan = _ic_get_chan(pin);

  // initialize IC timer channel
  TIM_ICInitTypeDef ic_init_struct;
  TIM_ICStructInit(&ic_init_struct);

  switch (chan) {
    case IC_CHAN1:
      ic_init_struct.TIM_Channel = TIM_Channel_1;
      break;
    case IC_CHAN2:
      ic_init_struct.TIM_Channel = TIM_Channel_2;
      break;
    case IC_CHAN3:
      ic_init_struct.TIM_Channel = TIM_Channel_3;
      break;
    case IC_CHAN4:
      ic_init_struct.TIM_Channel = TIM_Channel_4;
      break;
    default:
      //error
      ic_init_struct.TIM_Channel = 0xff;
      break;
  }

  ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  ic_init_struct.TIM_ICFilter = 0;

  TIM_ICInit(TIM4, &ic_init_struct);

  //calculate new prescaler if minimum frequency is lower than previous
  //frequencies
  // 72e6/2^16 = 1098.6328125
  uint16_t new_prescaler = (uint16_t) (ceilf(1098.6328125/min_freq) - 1);
  if (new_prescaler > _ge_ic_prescaler) {
    //change prescaler
    _ge_ic_prescaler = new_prescaler;

    TIM_PrescalerConfig(TIM4, _ge_ic_prescaler, TIM_PSCReloadMode_Immediate);
  }

  //enable pin
  GPIO_InitTypeDef gpio_struct;
  gpio_struct.GPIO_Mode = GPIO_Mode_AF;
  gpio_struct.GPIO_Pin = _ge_pin_num[pin];
  gpio_struct.GPIO_OType = GPIO_OType_PP;
  gpio_struct.GPIO_Speed = GPIO_Speed_10MHz;
  gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(_ge_pin_port[pin], &gpio_struct);

  uint8_t pin_af;
  switch (pin) {
    case PD12:
    case PD13:
    case PD14:
    case PD15:
    case PF6:
      pin_af = GPIO_AF_2;
      break;
    case PA11:
    case PA12:
    case PA13:
      pin_af = GPIO_AF_10;
      break;
    default:
      //error
      pin_af = 0xff;
  }

  GPIO_PinAFConfig(_ge_pin_port[pin], _ge_pin_source[pin], pin_af);

  return chan;
}


/**
 * @brief Get the elapsed period for the specified input capture pin
 * and return the instantaneous frequency.
 * @details Reads in the last captured period of the specified pin and
 * calculates the instanteous frequency. Note that this value may be
 * corrupted if the prescaler just changed.
 * 
 * @param pin Input capture pin to look at
 * @return The frequency in Hz
 */
float ic_read_freq(uint16_t pin) {
  ICTimerChan chan = _ic_get_chan(pin);

  float freq = 72.0e6/((float) _ge_ic_prescaler * (uint16_t)(_ge_ic_chan_captures[chan] - _ge_ic_chan_captures_last[chan]));

  return freq;
}


/**********Private Methods********************/

/**
 * @brief Returns the appropriate channel for the specified pin
 * @details Finds the associated timer channel for the pin from the
 * GE_IC_CHAN enumerated type.
 * 
 * @param pin Pin to look up
 * @return Input Capture timer channel
 */
ICTimerChan _ic_get_chan(uint16_t pin) {
  switch(pin) {
    //TIM4
    case PD12:
    case PA11: /* (Overlaps with TIM1_CHAN4) */
      return IC_CHAN1;
      break;
    case PD13:
    case PA12:
      return IC_CHAN2;
      break;
    case PA13:
    case PD14:
      return IC_CHAN3;
      break;
    case PD15:
    case PF6:
      return IC_CHAN4;
      break;
    default:
      //error
      return 0xff;
  }
}

/*****************Interrupt Handler*********************/
/**
 * @brief Handler for TIM4 global interrupt
 * @details TIM4 handler
 */
void TIM4_IRQHandler(void) {
  if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

    _ge_ic_chan_captures_last[IC_CHAN1] = _ge_ic_chan_captures[IC_CHAN1];
    _ge_ic_chan_captures[IC_CHAN1] = TIM_GetCapture1(TIM4);

    //reenable chan 1 IC
    TIM_ICInitTypeDef ic_init_struct;
    TIM_ICStructInit(&ic_init_struct);

    ic_init_struct.TIM_Channel = TIM_Channel_1;
    ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_init_struct.TIM_ICFilter = 0;

    TIM_ICInit(TIM4, &ic_init_struct);
    // _ge_ic_chan_captures[IC_CHAN1] = 3000;
  }

  if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

    _ge_ic_chan_captures_last[IC_CHAN2] = _ge_ic_chan_captures[IC_CHAN2];
    _ge_ic_chan_captures[IC_CHAN2] = TIM_GetCapture2(TIM4);

    //reenable chan 2 IC
    TIM_ICInitTypeDef ic_init_struct;
    TIM_ICStructInit(&ic_init_struct);

    ic_init_struct.TIM_Channel = TIM_Channel_2;
    ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_init_struct.TIM_ICFilter = 0;

    TIM_ICInit(TIM4, &ic_init_struct);
  }

  if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);

    _ge_ic_chan_captures_last[IC_CHAN3] = _ge_ic_chan_captures[IC_CHAN3];
    _ge_ic_chan_captures[IC_CHAN3] = TIM_GetCapture3(TIM4);

    //reenable chan 3 IC
    TIM_ICInitTypeDef ic_init_struct;
    TIM_ICStructInit(&ic_init_struct);

    ic_init_struct.TIM_Channel = TIM_Channel_3;
    ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_init_struct.TIM_ICFilter = 0;

    TIM_ICInit(TIM4, &ic_init_struct);
  }

  if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);

    _ge_ic_chan_captures_last[IC_CHAN4] = _ge_ic_chan_captures[IC_CHAN4];
    _ge_ic_chan_captures[IC_CHAN4] = TIM_GetCapture4(TIM4);

    //reenable chan 4 IC
    TIM_ICInitTypeDef ic_init_struct;
    TIM_ICStructInit(&ic_init_struct);

    ic_init_struct.TIM_Channel = TIM_Channel_4;
    ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_init_struct.TIM_ICFilter = 0;

    TIM_ICInit(TIM4, &ic_init_struct);
  }
}