/*
 * ee152_adc.c
 *
 *  Created on: Oct 3, 2014
 *      Author: Ned
 */

#include "ee152_adc.h"

//globals
int adc_pin_map[23] = {
    0, //ADC1_CHAN1
    1, //ADC1_CHAN2
    2, //ADC1_CHAN3
    3, //ADC1_CHAN4
    -1, //ADC1_CHAN5
    0, //ADC12_CHAN6
    1, //ADC12_CHAN7
    2, //ADC12_CHAN8
    3, //ADC12_CHAN9
    -1, //ADC12_CHAN10
    0, //ADC1_CHAN11
    1, //ADC1_CHAN12
    13, //ADC1_CHAN13
    4, //ADC2_CHAN1
    5, //ADC2_CHAN2
    6, //ADC2_CHAN3
    7, //ADC2_CHAN4
    4, //ADC2_CHAN5
    5, //ADC2_CHAN11
    2, //ADC2_CHAN12
    12, //ADC2_CHAN13
    14, //ADC2_CHAN14
    15 //ADC2_CHAN15
    };

GPIO_TypeDef *adc_bank_map[23] = {
    GPIOA, //ADC1_CHAN1
    GPIOA, //ADC1_CHAN2
    GPIOA, //ADC1_CHAN3
    GPIOA, //ADC1_CHAN4
    NULL, //ADC1_CHAN5
    GPIOC, //ADC12_CHAN6
    GPIOC, //ADC12_CHAN7
    GPIOC, //ADC12_CHAN8
    GPIOC, //ADC12_CHAN9
    NULL, //ADC12_CHAN10
    GPIOB, //ADC1_CHAN11
    GPIOB, //ADC1_CHAN12
    GPIOB, //ADC1_CHAN13
    GPIOA, //ADC2_CHAN1
    GPIOA, //ADC2_CHAN2
    GPIOA, //ADC2_CHAN3
    GPIOA, //ADC2_CHAN4
    GPIOC, //ADC2_CHAN5
    GPIOC, //ADC2_CHAN11
    GPIOB, //ADC2_CHAN12
    GPIOB, //ADC2_CHAN13
    GPIOB, //ADC2_CHAN14
    GPIOB //ADC2_CHAN15
    };


//private globals
int calibration_value_adc1, calibration_value_adc2;

int num_channels;
int curr_chan;

/*
 * This function initializes the ADCs in the STM32F3. This includes setting
 * the necessary clocks and global variables.
 */
//void adc_init(void) {
//  ADC_InitTypeDef       ADC_InitStructure;
//  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//  DMA_InitTypeDef DMA_InitStructure; //Variable used to setup the DMA
//
//  /* Configure the ADC clock */
//  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
//
//  /* Enable ADC1 and ADC2 clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
//
//  // Enable DMA clock
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//
//  // Enable TIM2 clock
//  RCC_AHBPeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//
//  /* ADC Channel configuration */
//  /* Set GPIO Periph clocks enable */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//
////  /* Configure ADC Channel7 as analog input */
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
////  GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//  ADC_StructInit(&ADC_InitStructure);
//
//  /* Calibration procedure */
//  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
//  ADC_VoltageRegulatorCmd(ADC2, ENABLE);
//
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul;
//  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;
//  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
//
//  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0; //TIM2, TRGO register
//  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
//  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
//  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
//  ADC_Init(ADC1, &ADC_InitStructure);
//  ADC_Init(ADC2, &ADC_InitStructure);
//
////  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul;
////  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
////  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
////  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
////  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;
////  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
////
////  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
////  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
////  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_11; //TIM2, TRGO register
////  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
////  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
////  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
////  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
////  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
////  ADC_Init(ADC1, &ADC_InitStructure);
////  ADC_Init(ADC2, &ADC_InitStructure);
//
////  //==Configure DMA1 - Channel1==
////  DMA_DeInit(DMA1_Channel1); //Set DMA registers to default values
////  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_2->CDR; //Address of peripheral the DMA must map to
////  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) data_buf; //Variable to which ADC values will be stored
////  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
////  DMA_InitStructure.DMA_BufferSize = 15; //Buffer size
////  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
////  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
////  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
////  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
////  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
////  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
////  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
////
////  DMA_Init(DMA1_Channel1, &DMA_InitStructure); //Initialise the DMA
////  DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
//
////  /* ADC1 regular channel7 configuration */
////  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_7Cycles5);
//
//  num_channels = 0;
//  for (int i=0; i<15; i++)
//    adc_reg_callbacks[i] = NULL;
//
//  //enable interrupts
//  ADC_ITConfig(ADC1, ADC_IT_EOS, ENABLE);
//  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
//  ADC_ITConfig(ADC2, ADC_IT_EOS, ENABLE);
//  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
//  NVIC_InitTypeDef NVIC_InitStructure;
//
//  /* Enable the TIM3 gloabal Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

void adc_enable_clocks(void) {
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure the ADC clock */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);

  /* ADC1 Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

  // enable TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void adc_init(void) {
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  __IO uint16_t calibration_value = 0;

  //enable clocks
  adc_enable_clocks();

//  /* Setup SysTick Timer for 1 µsec interrupts  */
//  if(SysTick_Config(SystemCoreClock / 1000000)){
//    for(;;);
//  }

//  /* ADC Channel configuration */
//  /* Configure ADC Channel7 (PC1) as analog input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);

  ADC_StructInit(&ADC_InitStructure);

  /* Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);

  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  calibration_value_adc1 = ADC_GetCalibrationValue(ADC1);

  ADC_VoltageRegulatorCmd(ADC2, ENABLE);

  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC2);

  while(ADC_GetCalibrationStatus(ADC2) != RESET );
  calibration_value_adc2 = ADC_GetCalibrationValue(ADC2);

  /* Configure the ADC1 and ADC2 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC2, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_11;
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC1 regular channel7 configuration */
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_7Cycles5);

  num_channels = 0;
  curr_chan = 0;
  for (int i=0; i<15; i++)
    adc_reg_callbacks[i] = NULL;

  /* Enable End Of Conversion and End of Sequence interrupts */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_ITConfig(ADC1, ADC_IT_EOS, ENABLE);
//  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
//  ADC_ITConfig(ADC2, ADC_IT_EOS, ENABLE);

  /* Configure and enable ADC1 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void adc_deinit(void) {
  adc_stop();

  //reset ADC
  ADC_DeInit(ADC1);
  ADC_DeInit(ADC2);

  num_channels = 0;
  for (int i=0; i<15; i++)
    adc_reg_callbacks[i] = NULL;
}

//startup ADCs
void adc_start(void) {
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);

  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));

//  /* TIM1 counter enable */
//  TIM_Cmd(TIM1, ENABLE);
//
//  /* TIM1 main Output Enable */
//  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  /* doesn't return ? */

//  /* Enable ADC1 */
//  ADC_Cmd(ADC1, ENABLE);
//  ADC_Cmd(ADC2, ENABLE);
//
//  ADC_StartConversion(ADC1);
//  ADC_StartConversion(ADC2);

//  /* wait for ADRDY */
//  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
//  while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));

//  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
//  ADC_StartCalibration(ADC1);
//
//  while(ADC_GetCalibrationStatus(ADC1) != RESET );
//  calibration_value_adc1 = ADC_GetCalibrationValue(ADC1);
//
//  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
//  ADC_StartCalibration(ADC2);
//
//  while(ADC_GetCalibrationStatus(ADC2) != RESET );
//  calibration_value_adc2 = ADC_GetCalibrationValue(ADC2);

  /* ADC1 Start Conversion */
   ADC_StartConversion(ADC1);
}

void adc_stop(void) {
  /* Disable ADC1 and ADC2 */
  ADC_StopConversion(ADC1);
  ADC_StopConversion(ADC2);

  ADC_Cmd(ADC1, DISABLE);
  ADC_Cmd(ADC2, DISABLE);
}

//input channel to enable (between 1 and 16)
//the order the channels are initialized dictates the order they are converted
//this function enables the channel on both ADC1 and ADC2
void adc_enable_channel(uint16_t chan) {
  ADC_InitTypeDef       ADC_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  //add to total channel count
  num_channels += 1;


//  //disable ADCs
//  ADC_Cmd(ADC1, DISABLE);
//  ADC_Cmd(ADC2, DISABLE);

  int adc1_pin, adc2_pin;
  GPIO_TypeDef *adc1_bank;
  GPIO_TypeDef *adc2_bank;

  if (chan >= 1 && chan <= 5) {
    adc1_pin = adc_pin_map[chan - 1];
    adc1_bank = adc_bank_map[chan - 1];
    adc2_pin = adc_pin_map[chan - 1 + 14];
    adc2_bank = adc_bank_map[chan - 1 + 14];
  } else if (chan >= 11 && chan <= 13) {
    adc1_pin = adc_pin_map[chan-1];
    adc1_bank = adc_bank_map[chan-1];
    adc2_pin = adc_pin_map[chan-1+8];
    adc2_bank = adc_bank_map[chan-1+8];
  } else if (chan >= 14 && chan <= 15) {
    adc1_pin = -1;
    adc1_bank = NULL;
    adc2_pin = adc_pin_map[chan-1+8];
    adc2_bank = adc_bank_map[chan-1+8];
  } else {
    // incorrect channel
    // do nothing
    return;
  }

  chan_order[num_channels-1] = chan;

  /* Configure ADC1 channel as analog input */
  GPIO_InitStructure.GPIO_Pin = 0x1 << adc1_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(adc1_bank, &GPIO_InitStructure);

  /* Configure ADC2 channel as analog input */
  GPIO_InitStructure.GPIO_Pin = 0x1 << adc2_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(adc2_bank, &GPIO_InitStructure);

  /* ADC1 regular channel configuration */
  ADC_RegularChannelConfig(ADC1, chan, num_channels, ADC_SampleTime_7Cycles5);

  /* ADC2 regular channel configuration */
  ADC_RegularChannelConfig(ADC2, chan, num_channels, ADC_SampleTime_7Cycles5);

  // Configure ADCs
  ADC_RegularChannelSequencerLengthConfig(ADC1, num_channels);
  ADC_RegularChannelSequencerLengthConfig(ADC2, num_channels);

//  // Enable ADCs
//  ADC_Cmd(ADC1, ENABLE);
//  ADC_Cmd(ADC2, ENABLE);
}

//attach callback function to a specific channel
void adc_callback(const int chan, void (*callback)(uint32_t)) {
  adc_reg_callbacks[chan-1] = callback;
}

//set sampling frequency
void adc_set_fs(float fs) {
  uint16_t num_ticks = (uint16_t) (72000000.0/fs);

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  //configure timer
  /* Compute the prescaler value */
  uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock) / 72000000) - 1;

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = num_ticks-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); // ADC_ExternalTrigConv_T2_TRGO

  /* Enables the TIM2 counter */
  TIM_Cmd(TIM2, ENABLE);

//  /* TIM1 configuration ------------------------------------------------------*/
//  /* Time Base configuration */
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  TIM_TimeBaseStructure.TIM_Period = 150 - 1;
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//
//  /* TIM1 channel1 configuration in PWM mode */
//  TIM_OCStructInit(&TIM_OCInitStructure);
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 150 / 2;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//
//  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
}

void ADC1_2_IRQHandler(void) {
  //ADC 1
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
    data_buf[curr_chan] = ADC_GetDualModeConversionValue(ADC1);
    curr_chan++;
  }
  if (ADC_GetITStatus(ADC1, ADC_IT_EOS)) {
    curr_chan = 0;
    //sequence has finished. now need to send data to appropriate callbacks
    for (int i=0; i<num_channels; i++) {
      int chan = chan_order[i];
      //pass data to registered callback
      if (adc_reg_callbacks[chan-1])
        adc_reg_callbacks[chan-1](data_buf[i]);
    }
  }

  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  ADC_ClearITPendingBit(ADC1, ADC_IT_EOS);
//  //ADC 2
//  if (ADC_GetITStatus(ADC2, ADC_IT_EOC)) {
//    int data = ADC_GetConversionValue(ADC2);
//    ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
//  }
//  if (ADC_GetITStatus(ADC2, ADC_IT_EOS)) {
//    //sequence has finished. now need to send data to appropriate callbacks
//    for (int i=0; i<num_channels; i++) {
//      int chan = chan_order[i];
//      //pass data to registered callback
//      if (adc_reg_callbacks[chan])
//        adc_reg_callbacks[chan](data_buf[i]);
//    }
//    ADC_ClearITPendingBit(ADC2, ADC_IT_EOS);
//  }
}
