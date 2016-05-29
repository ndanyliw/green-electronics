/*
 * ge_adc.c
 *
 *  Created on: Oct 3, 2014
 *      Author: Ned
 */

#include "ge_adc.h"

//globals
GPIOPin adc_pin_map[NUM_ADC] = {
  PA0, //ADC1_1
  PA1, //ADC1_2
  PA2, //ADC1_3
  PA3, //ADC1_4
  PF4, //ADC1_5
  PA4, //ADC2_1
  PA5, //ADC2_2
  PA6, //ADC2_3
  PA7, //ADC2_4
  PC4, //ADC2_5
  PC0, //ADC12_6
  PC1, //ADC12_7
  PC2, //ADC12_8
  PC3, //ADC12_9
  PF2, //ADC12_10
  PC5, //ADC2_11
  PB2, //ADC2_12
  PB1, //ADC3_1
  PE9, //ADC3_2
  PE13, //ADC3_3
  PB13, //ADC3_5
  PB0, //ADC3_12
  PE7, //ADC3_13
  PE10, //ADC3_14
  PE11, //ADC3_15
  PE12, //ADC3_16
  PE8, //ADC34_6
  PD10, //ADC34_7
  PD11, //ADC34_8
  PD12, //ADC34_9
  PD13, //ADC34_10
  PD14, //ADC34_11
  PE14, //ADC4_1
  PE15, //ADC4_2
  PB12, //ADC4_3
  PB14, //ADC4_4
  PB15, //ADC4_5
  PD8, //ADC4_12
  PD9 //ADC4_13
  };

ADC_TypeDef *adc_bank_map[NUM_ADC] = {
  ADC1, //ADC1_1
  ADC1, //ADC1_2
  ADC1, //ADC1_3
  ADC1, //ADC1_4
  ADC1, //ADC1_5
  ADC2, //ADC2_1
  ADC2, //ADC2_2
  ADC2, //ADC2_3
  ADC2, //ADC2_4
  ADC2, //ADC2_5
  ADC1, //ADC12_6
  ADC1, //ADC12_7
  ADC1, //ADC12_8
  ADC1, //ADC12_9
  ADC1, //ADC12_10
  ADC2, //ADC2_11
  ADC2, //ADC2_12
  ADC3, //ADC3_1
  ADC3, //ADC3_2
  ADC3, //ADC3_3
  ADC3, //ADC3_5
  ADC3, //ADC3_12
  ADC3, //ADC3_13
  ADC3, //ADC3_14
  ADC3, //ADC3_15
  ADC3, //ADC3_16
  ADC3, //ADC34_6
  ADC3, //ADC34_7
  ADC3, //ADC34_8
  ADC3, //ADC34_9
  ADC3, //ADC34_10
  ADC3, //ADC34_11
  ADC4, //ADC4_1
  ADC4, //ADC4_2
  ADC4, //ADC4_3
  ADC4, //ADC4_4
  ADC4, //ADC4_5
  ADC4, //ADC4_12
  ADC4 //ADC4_13
  };

ADC_TypeDef *adc_chan_map[NUM_ADC] = {
  ADC_Channel_1, //ADC1_1
  ADC_Channel_2, //ADC1_2
  ADC_Channel_3, //ADC1_3
  ADC_Channel_4, //ADC1_4
  ADC_Channel_5, //ADC1_5
  ADC_Channel_1, //ADC2_1
  ADC_Channel_2, //ADC2_2
  ADC_Channel_3, //ADC2_3
  ADC_Channel_3, //ADC2_4
  ADC_Channel_5, //ADC2_5
  ADC_Channel_6, //ADC12_6
  ADC_Channel_7, //ADC12_7
  ADC_Channel_8, //ADC12_8
  ADC_Channel_9, //ADC12_9
  ADC_Channel_10, //ADC12_10
  ADC_Channel_11, //ADC2_11
  ADC_Channel_12, //ADC2_12
  ADC_Channel_1, //ADC3_1
  ADC_Channel_2, //ADC3_2
  ADC_Channel_3, //ADC3_3
  ADC_Channel_5, //ADC3_5
  ADC_Channel_12, //ADC3_12
  ADC_Channel_13, //ADC3_13
  ADC_Channel_14, //ADC3_14
  ADC_Channel_15, //ADC3_15
  ADC_Channel_16, //ADC3_16
  ADC_Channel_6, //ADC34_6
  ADC_Channel_7, //ADC34_7
  ADC_Channel_8, //ADC34_8
  ADC_Channel_9, //ADC34_9
  ADC_Channel_10, //ADC34_10
  ADC_Channel_11, //ADC34_11
  ADC_Channel_1, //ADC4_1
  ADC_Channel_2, //ADC4_2
  ADC_Channel_3, //ADC4_3
  ADC_Channel_4, //ADC4_4
  ADC_Channel_5, //ADC4_5
  ADC_Channel_12, //ADC4_12
  ADC_Channel_13 //ADC4_13
  };


//private globals
int calibration_value_adc1, calibration_value_adc2, calibration_value_adc3,
  calibration_value_adc4;

int num_channels1, num_channels2, num_channels3, num_channels4;
__IO int curr_chan1, curr_chan2, curr_chan3, curr_chan4;

// state of ADCs (each bit is a flag with 0 = ready and 1 = done)
__IO uint8_t adc_state;
// flag to mark data overflow
__IO uint8_t _ge_adc_ovf;

// arrays to hold conversion values from ADCs
__IO uint16_t data_buf1[16], data_buf2[16], data_buf3[16], data_buf4[16];

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
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);


  /* Configure the ADC clock */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);

  /* ADC1 Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC34, ENABLE);

  // enable TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}


void adc_init(void) {
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  __IO uint16_t calibration_value = 0;

  //enable clocks
  adc_enable_clocks();

  ADC_StructInit(&ADC_InitStructure);

  /* Calibration procedure */
  // ADC1
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);

  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1) != RESET);
  calibration_value_adc1 = ADC_GetCalibrationValue(ADC1);

  //ADC2
  ADC_VoltageRegulatorCmd(ADC2, ENABLE);

  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC2);

  while(ADC_GetCalibrationStatus(ADC2) != RESET);
  calibration_value_adc2 = ADC_GetCalibrationValue(ADC2);


  //ADC3
  ADC_VoltageRegulatorCmd(ADC3, ENABLE);

  ADC_SelectCalibrationMode(ADC3, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC3);

  while(ADC_GetCalibrationStatus(ADC3) != RESET);
  calibration_value_adc3 = ADC_GetCalibrationValue(ADC3);

  //ADC4
  ADC_VoltageRegulatorCmd(ADC4, ENABLE);

  ADC_SelectCalibrationMode(ADC4, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC4);

  while(ADC_GetCalibrationStatus(ADC4) != RESET);
  calibration_value_adc4 = ADC_GetCalibrationValue(ADC4);

  /* Configure ADC1, ADC2, ADC3, and ADC4 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC2, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC3, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC4, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_11;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel = 1;

  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_Init(ADC4, &ADC_InitStructure);

  num_channels = 0;
  curr_chan = 0;
  for (int i = 0; i < 16; i++)
    adc_reg_callbacks[i] = NULL;

  /* Enable End of Conversion and End of Sequence interrupts */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_ITConfig(ADC1, ADC_IT_EOS, ENABLE);
  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);
  ADC_ITConfig(ADC2, ADC_IT_EOS, ENABLE);
  ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
  ADC_ITConfig(ADC3, ADC_IT_EOS, ENABLE);
  ADC_ITConfig(ADC4, ADC_IT_EOC, ENABLE);
  ADC_ITConfig(ADC4, ADC_IT_EOS, ENABLE);

  /* Configure and enable ADC1 and ADC2 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable ADC3 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable ADC4 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void adc_deinit(void) {
  adc_stop();

  //reset ADCs
  ADC_DeInit(ADC1);
  ADC_DeInit(ADC2);
  ADC_DeInit(ADC3);
  ADC_DeInit(ADC4);

  num_channels = 0;
  for (int i=0; i<16; i++) {
    adc_reg_callbacks1[i] = NULL;
    adc_reg_callbacks2[i] = NULL;
    adc_reg_callbacks3[i] = NULL;
    adc_reg_callbacks4[i] = NULL;
  }
}

//startup ADCs
void adc_start(void) {
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
  ADC_Cmd(ADC4, ENABLE);

  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));
  while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_RDY));
  while(!ADC_GetFlagStatus(ADC4, ADC_FLAG_RDY));

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
   ADC_StartConversion(ADC2);
   ADC_StartConversion(ADC3);
   ADC_StartConversion(ADC4);
}

void adc_stop(void) {
  /* Disable ADC1 and ADC2 */
  ADC_StopConversion(ADC1);
  ADC_StopConversion(ADC2);
  ADC_StopConversion(ADC3);
  ADC_StopConversion(ADC4);

  ADC_Cmd(ADC1, DISABLE);
  ADC_Cmd(ADC2, DISABLE);
  ADC_Cmd(ADC3, DISABLE);
  ADC_Cmd(ADC4, DISABLE);
}


//input channel to enable (between 1 and 16)
//the order the channels are initialized dictates the order they are converted
//this function enables the channel on both ADC1 and ADC2
void adc_enable_channel(uint16_t chan) {
  ADC_InitTypeDef       ADC_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  ADC_TypeDef *sel_adc;

  // select appropriate ADC
  if (chan >= ADC1_1 && chan <= ADC1_5) {
    sel_adc = ADC1;
  } else if (chan >= ADC2_1 && chan <= ADC2_5) {
    sel_adc = ADC2;
  } else if (chan >= ADC12_6 && chan <= ADC12_10) {
    sel_adc = ADC1;
  } else if (chan >= ADC2_11 && chan <= ADC2_12) {
    sel_adc = ADC2;
  } else if (chan >= ADC3_1 && chan <= ADC3_16) {
    sel_adc = ADC3;
  } else if (chan >= ADC34_6 && chan <= ADC34_11) {
    sel_adc = ADC3;
  } else if (chan >= ADC4_1 && chan <= ADC4_13) {
    sel_adc = ADC4;
  } else {
    sel_adc = NULL;
  }

  // add to channel count
  if (sel_adc == ADC1) {
    num_channels1++;
  } else if (sel_adc == ADC2) {
    num_channels2++;
  } else if (sel_adc == ADC3) {
    num_channels3++;
  } else if (sel_adc == ADC4) {
    num_channels4++;
  }


//  //disable ADCs
//  ADC_Cmd(ADC1, DISABLE);
//  ADC_Cmd(ADC2, DISABLE);

  int adc_pin = adc_pin_map[chan];
  int adc_gpio = _ge_pin_number[adc_pin];
  int adc_port = _ge_pin_port[adc_pin];

  /* Configure ADC channel as analog input */
  GPIO_InitStructure.GPIO_Pin = adc_gpio;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(adc_port, &GPIO_InitStructure);

  // /* ADC1 regular channel configuration */
  // ADC_RegularChannelConfig(ADC1, chan, num_channels, ADC_SampleTime_7Cycles5);

  // /* ADC2 regular channel configuration */
  // ADC_RegularChannelConfig(ADC2, chan, num_channels, ADC_SampleTime_7Cycles5);

  // // Configure ADCs
  // ADC_RegularChannelSequencerLengthConfig(ADC1, num_channels);
  // ADC_RegularChannelSequencerLengthConfig(ADC2, num_channels);

//  // Enable ADCs
//  ADC_Cmd(ADC1, ENABLE);
//  ADC_Cmd(ADC2, ENABLE);
}

// get ADC conversion sequence
// TODO write get and set method for adc conversion method and then
// write a method to initialize adcs based on this list
int *adc_get_conversion_order(ADC_TypeDef *ADCx) {
  if (ADCx == ADC1) {
    return chan_order1;
  } else if (ADCx == ADC2) {
    return chan_order2;
  } else if (ADCx == ADC3) {
    return chan_order3;
  } else if (ADCx == ADC4) {
    return chan_order4;
  } else {
    return NULL;
  }
}


// void adc_set_conversion_order(ADC_TypeDef *ADCx, int *order) {
//   int *chan_to_set;
//   if (ADCx == ADC1) {
//     chan_to_set = chan_order1;
//   } else if (ADCx == ADC2) {
//     chan_to_set = chan_order2;
//   } else if (ADCx == ADC3) {
//     chan_to_set = chan_order3;
//   } else if (ADCx == ADC4) {
//     chan_to_set = chan_order4;
//   } else {
//     return NULL;
//   }
  
//   for (int k = 0; k < 16; k++) {
//     chan_to_set[k] = order[k];
//   }
// }

// TODO: need to enable the actual conversion sequences on the ADC. also make sure
// that the conversions aren't continuous
void adc_enable_channels(ADC_CHAN_Type *channels, uint16_t num_conv) {
  for (int k = 0; k < num_conv; k++) {
    // adc_conv_order[k] = channels[k];
    ADC_CHAN_Type sel_adc_chan = channels[k];

    adc_conv_order[k].pin = adc_pin_map[sel_adc_chan];
    adc_conv_order[k].chan = sel_adc_chan;
    adc_conv_order[k].STM_ADCx = adc_bank_map[sel_adc_chan];
    adc_conv_order[k].STM_ADC_chan = adc_chan_map[sel_adc_chan];
    adc_conv_order[k].enabled = true;
    adc_conv_order[k].period = 1;
  }
}


//this method assumes that you have already populated the global ADC channel
//info list and initializes all the appropriate GPIO and ADC info. checks to see
//if the channels are supposed to be enabled
void adc_initialize_channels() {
  for (int k = 0; k < NUM_ADC; k++) {
    if (adc_conv_order[k].enabled) {
      GPIOPin pin = adc_conv_order[k].pin;

      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = _ge_pin_num[pin];
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(_ge_pin_port[pin], &GPIO_InitStruct);

      if (adc_conv_order[k].STM_ADCx == ADC1) {
        num_chan_adc1++;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc1, ADC_SampleTime_7Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC2) {
        num_chan_adc2++;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc2, ADC_SampleTime_7Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC3) {
        num_chan_adc3++;
        
        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc3, ADC_SampleTime_7Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC4) {
        num_chan_adc4++;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc4, ADC_SampleTime_7Cycles5);
      } else {
        return;
      }
    }
  }

  // enable conversion sequences for ADCs
  ADC_RegularChannelSequencerLengthConfig(ADC1, num_chan_adc1);
  ADC_RegularChannelSequencerLengthConfig(ADC2, num_chan_adc2);
  ADC_RegularChannelSequencerLengthConfig(ADC3, num_chan_adc3);
  ADC_RegularChannelSequencerLengthConfig(ADC4, num_chan_adc4);

  // enable ADCs
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
  ADC_Cmd(ADC4, ENABLE);
}

// //attach callback function to a specific channel
// void adc_callback(const int chan, void (*callback)(uint32_t)) {
//   adc_reg_callbacks[chan-1] = callback;
// }

void adc_callback(const 

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

uint8_t adc_get_state() {
  return adc_state;
}

uint8_t adc_get_ovf() {
  return _ge_adc_ovf;
}

uint8_t adc_clear_ovf() {
  _ge_adc_ovf = 0;
}

void ADC1_2_IRQHandler(void) {
  // //ADC 1
  // if (ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
  //   data_buf[curr_chan] = ADC_GetDualModeConversionValue(ADC1);
  //   curr_chan++;
  // }
  // if (ADC_GetITStatus(ADC1, ADC_IT_EOS)) {
  //   curr_chan = 0;
  //   //sequence has finished. now need to send data to appropriate callbacks
  //   for (int i=0; i<num_channels; i++) {
  //     int chan = chan_order[i];
  //     //pass data to registered callback
  //     if (adc_reg_callbacks[chan-1])
  //       adc_reg_callbacks[chan-1](data_buf[i]);
  //   }
  // }

  // ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  // ADC_ClearITPendingBit(ADC1, ADC_IT_EOS);
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

  //ADC 1
  if(ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
    data_buf1[curr_chan1] = ADC_GetConversionValue(ADC1);
    curr_chan1++;

    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
  if(ADC_GetITStatus(ADC1, ADC_IT_EOS)) {
    curr_chan1 = 0;

    // check for overflow
    if(adc_state == 0xf) _ge_adc_ovf = 1;

    // mark sequence as finished
    adc_state |= 0x1 << 0;

    if(adc_state == 0xf) adc_reg_callback(data_buf1, data_buf2, data_buf3,
                                          data_buf4);

    ADC_ClearITPendingBit(ADC1, ADC_IT_EOS);
  }

  //ADC 2
  if(ADC_GetITStatus(ADC2, ADC_IT_EOC)) {
    data_buf2[curr_chan2] = ADC_GetConversionValue(ADC2);
    curr_chan2++;

    ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
  }
  if(ADC_GetITStatus(ADC2, ADC_IT_EOS)) {
    curr_chan2 = 0;

    // check for overflow
    if(adc_state == 0xf) _ge_adc_ovf = 1;

    // mark sequence as finished
    adc_state |= 0x1 << 1;

    // call callback if all sequences finished
    if(adc_state == 0xf) adc_reg_callback(data_buf1, data_buf2, data_buf3,
                                          data_buf4);

    ADC_ClearITPendingBit(ADC2, ADC_IT_EOS);
  }
}


//handler for ADC3 interupt 
void ADC3_IRQHandler(void) {
  //ADC 3
  if(ADC_GetITStatus(ADC3, ADC_IT_EOC)) {
    data_buf3[curr_chan3] = ADC_GetConversionValue(ADC3);
    curr_chan3++;

    ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
  }
  if(ADC_GetITStatus(ADC3, ADC_IT_EOS)) {
    curr_chan3 = 0;

    // check for overflow
    if(adc_state == 0xf) _ge_adc_ovf = 1;

    // mark sequence as finished
    adc_state |= 0x1 << 2;

    // call callback if all sequences finished
    if(adc_state == 0xf) adc_reg_callback(data_buf1, data_buf2, data_buf3,
                                          data_buf4);

    ADC_ClearITPendingBit(ADC3, ADC_IT_EOS);
  }
}

//handler for ADC3 interupt 
void ADC4_IRQHandler(void) {
  //ADC 4
  if(ADC_GetITStatus(ADC4, ADC_IT_EOC)) {
    data_buf4[curr_chan4] = ADC_GetConversionValue(ADC4);
    curr_chan4++;

    ADC_ClearITPendingBit(ADC4, ADC_IT_EOC);
  }
  if(ADC_GetITStatus(ADC4, ADC_IT_EOS)) {
    curr_chan4 = 0;

    // check for overflow
    if(adc_state == 0xf) _ge_adc_ovf = 1;

    // mark sequence as finished
    adc_state |= 0x1 << 3;

    // call callback if all sequences finished
    if(adc_state == 0xf) adc_reg_callback(data_buf1, data_buf2, data_buf3,
                                          data_buf4);

    ADC_ClearITPendingBit(ADC4, ADC_IT_EOS);
  }
}

