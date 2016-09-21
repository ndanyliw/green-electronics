/**
 * @file  ge_ic_int.c
 * @brief Implementation of input capture library using software interrupts
 * 
 * @author Ned Danyliw
 * @date  9.2016
 */

#include "ge_ic_int.h"
#include "ge_pins.h"
#include <string.h>

//private variables
static __IO uint32_t _ge_ic_int_count;
static __IO bool _ge_ic_int_ovf;


/**
 * @brief Initialize input capture timer
 * @details Initializes TIM4 for the input capture library and
 * sets PB10 as an input.
 */
void ic_int_init() {
  //initialize variables
  _ge_ic_int_count = 0;
  _ge_ic_int_ovf = false;


  //enable clocks for TIM4 and GPIOB
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  //enable GPIO
  gpio_init();

  TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
  TIM_ICInitTypeDef TIM_IC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //setup TIM4
  TIM_TimeBaseStructInit(&TIM_TimeBase_InitStructure);
  TIM_ICStructInit(&TIM_IC_InitStructure);

  TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBase_InitStructure.TIM_Period = 0xffff;
  TIM_TimeBase_InitStructure.TIM_Prescaler = 0x001ff; // 72 Mhz / 512 = 140.625 kHz
  TIM_TimeBase_InitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBase_InitStructure);

  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  //setup NVIC for TIM4
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  //enable interrupts
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  //enable PB10 as input capture pin
  GPIO_InitTypeDef gpio_struct;
  GPIO_StructInit(&gpio_struct);

  gpio_struct.GPIO_Mode = GPIO_Mode_IN;
  gpio_struct.GPIO_Pin = _ge_pin_num[PB10];
  gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(_ge_pin_port[PB10], &gpio_struct);

  //Attach interrupt to PB10
  EXTI_InitTypeDef EXTI_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);

  //setup NVIC for External interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  EXTI_StructInit(&EXTI_InitStruct);
  /* PB10 is connected to EXTI_Line10 */
  EXTI_InitStruct.EXTI_Line = EXTI_Line10;
  /* Enable interrupt */
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  /* Interrupt mode */
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Triggers on rising and falling edge */
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  /* Add to EXTI */
  EXTI_Init(&EXTI_InitStruct);

  //enable timer
  TIM_Cmd(TIM4, ENABLE);
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
float ic_int_read_freq() {
  // copy result so it isn't updated during frequency calculation
  uint32_t count;
  memcpy(&count, (const void *)&_ge_ic_int_count, 4);

  if (count == 0 || _ge_ic_int_ovf)
    return 0.0;

  //magic number - 72MHz/512 prescaler
  return 140625.0 / (count);
}



/*****************Interrupt Handler*********************/
/**
 * @brief Handler for TIM4 global interrupt
 * @details TIM4 handler. Sets minimum frequency that can be
 * measured.
 */
void TIM4_IRQHandler(void) {

  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
    //if the count overflows assume frequency is 0
    _ge_ic_int_ovf = true;
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}


/**
 * @brief Handler for external interrupt
 * @details Handler calculates how many timer ticks have occured
 * since last external interrupt event. Resets the timer count
 * to 0 after interrupt.
 */
void EXTI15_10_IRQHandler(void) {
  /* Make sure that interrupt flag is set */
  if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
    /* Do your stuff when PB10 is changed */
    //check if overflowed
    if (_ge_ic_int_ovf) {
      //set count as 0
      _ge_ic_int_count = 0;
    } else {
      //get count
      _ge_ic_int_count = TIM_GetCounter(TIM4);
    }
    
    //clear overflow flag
    _ge_ic_int_ovf = false;
    //reset count
    TIM_SetCounter(TIM4, 0);

    /* Clear interrupt flag */
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
}