/**
 * @file ge_adc.c
 * @brief Library to interface with the ADCs on the STM32
 * microcontroller and handle the DMA transferes for storing
 * the results.
 * 
 * @author Ned Danyliw
 * @date 9.2016
 */

#include "ge_adc.h"

// Global look-up tables
// Map what physical channel each channel corresponds to
GPIOPin adc_pin_map[GE_NUM_ADC] = {
  PA0, //GE_ADC1_1
  PA1, //GE_ADC1_2
  PA2, //GE_ADC1_3
  PA3, //GE_ADC1_4
  PF4, //GE_ADC1_5
  PA4, //GE_ADC2_1
  PA5, //GE_ADC2_2
  PA6, //GE_ADC2_3
  PA7, //GE_ADC2_4
  PC4, //GE_ADC2_5
  PC0, //GE_ADC12_6
  PC1, //GE_ADC12_7
  PC2, //GE_ADC12_8
  PC3, //GE_ADC12_9
  PF2, //GE_ADC12_10
  PC5, //GE_ADC2_11
  PB2, //GE_ADC2_12
  PB1, //GE_ADC3_1
  PE9, //GE_ADC3_2
  PE13, //GE_ADC3_3
  PB13, //GE_ADC3_5
  PB0, //GE_ADC3_12
  PE7, //GE_ADC3_13
  PE10, //GE_ADC3_14
  PE11, //GE_ADC3_15
  PE12, //GE_ADC3_16
  PE8, //GE_ADC34_6
  PD10, //GE_ADC34_7
  PD11, //GE_ADC34_8
  PD12, //GE_ADC34_9
  PD13, //GE_ADC34_10
  PD14, //GE_ADC34_11
  PE14, //GE_ADC4_1
  PE15, //GE_ADC4_2
  PB12, //GE_ADC4_3
  PB14, //GE_ADC4_4
  PB15, //GE_ADC4_5
  PD8, //GE_ADC4_12
  PD9 //GE_ADC4_13
  };

// Map what ADC is attached to each channel
ADC_TypeDef *adc_bank_map[GE_NUM_ADC] = {
  ADC1, //GE_ADC1_1
  ADC1, //GE_ADC1_2
  ADC1, //GE_ADC1_3
  ADC1, //GE_ADC1_4
  ADC1, //GE_ADC1_5
  ADC2, //GE_ADC2_1
  ADC2, //GE_ADC2_2
  ADC2, //GE_ADC2_3
  ADC2, //GE_ADC2_4
  ADC2, //GE_ADC2_5
  ADC1, //GE_ADC12_6
  ADC1, //GE_ADC12_7
  ADC1, //GE_ADC12_8
  ADC1, //GE_ADC12_9
  ADC1, //GE_ADC12_10
  ADC2, //GE_ADC2_11
  ADC2, //GE_ADC2_12
  ADC3, //GE_ADC3_1
  ADC3, //GE_ADC3_2
  ADC3, //GE_ADC3_3
  ADC3, //GE_ADC3_5
  ADC3, //GE_ADC3_12
  ADC3, //GE_ADC3_13
  ADC3, //GE_ADC3_14
  ADC3, //GE_ADC3_15
  ADC3, //GE_ADC3_16
  ADC3, //GE_ADC34_6
  ADC3, //GE_ADC34_7
  ADC3, //GE_ADC34_8
  ADC3, //GE_ADC34_9
  ADC3, //GE_ADC34_10
  ADC3, //GE_ADC34_11
  ADC4, //GE_ADC4_1
  ADC4, //GE_ADC4_2
  ADC4, //GE_ADC4_3
  ADC4, //GE_ADC4_4
  ADC4, //GE_ADC4_5
  ADC4, //GE_ADC4_12
  ADC4 //GE_ADC4_13
  };

// Map what channel is attached to each ADC channel
uint8_t adc_chan_map[GE_NUM_ADC] = {
  ADC_Channel_1, //GE_ADC1_1
  ADC_Channel_2, //GE_ADC1_2
  ADC_Channel_3, //GE_ADC1_3
  ADC_Channel_4, //GE_ADC1_4
  ADC_Channel_5, //GE_ADC1_5
  ADC_Channel_1, //GE_ADC2_1
  ADC_Channel_2, //GE_ADC2_2
  ADC_Channel_3, //GE_ADC2_3
  ADC_Channel_3, //GE_ADC2_4
  ADC_Channel_5, //GE_ADC2_5
  ADC_Channel_6, //GE_ADC12_6
  ADC_Channel_7, //GE_ADC12_7
  ADC_Channel_8, //GE_ADC12_8
  ADC_Channel_9, //GE_ADC12_9
  ADC_Channel_10, //GE_ADC12_10
  ADC_Channel_11, //GE_ADC2_11
  ADC_Channel_12, //GE_ADC2_12
  ADC_Channel_1, //GE_ADC3_1
  ADC_Channel_2, //GE_ADC3_2
  ADC_Channel_3, //GE_ADC3_3
  ADC_Channel_5, //GE_ADC3_5
  ADC_Channel_12, //GE_ADC3_12
  ADC_Channel_13, //GE_ADC3_13
  ADC_Channel_14, //GE_ADC3_14
  ADC_Channel_15, //GE_ADC3_15
  ADC_Channel_16, //GE_ADC3_16
  ADC_Channel_6, //GE_ADC34_6
  ADC_Channel_7, //GE_ADC34_7
  ADC_Channel_8, //GE_ADC34_8
  ADC_Channel_9, //GE_ADC34_9
  ADC_Channel_10, //GE_ADC34_10
  ADC_Channel_11, //GE_ADC34_11
  ADC_Channel_1, //GE_ADC4_1
  ADC_Channel_2, //GE_ADC4_2
  ADC_Channel_3, //GE_ADC4_3
  ADC_Channel_4, //GE_ADC4_4
  ADC_Channel_5, //GE_ADC4_5
  ADC_Channel_12, //GE_ADC4_12
  ADC_Channel_13 //GE_ADC4_13
  };


// Private globals
// Offsets from ADC calibration procedures on startup
int calibration_value_adc1, calibration_value_adc2, calibration_value_adc3,
  calibration_value_adc4;

// state of ADC conversions (each bit is a flag with 0 = ready and 1 = done)
__IO uint8_t adc_state;
// state that indicates all conversions have finished
uint8_t adc_val_rdy_state;
// flag to mark data overflow
__IO uint8_t _ge_adc_ovf;

// arrays to hold conversion values from ADCs
__IO uint32_t data_buf1[16], data_buf2[16], data_buf3[16], data_buf4[16];

// arrays to handle results in the correct order
__IO uint16_t adc_conversions[GE_NUM_ADC];
__IO uint16_t adc1_conv_map[16], adc2_conv_map[16], adc3_conv_map[16], adc4_conv_map[16];

// total number of conversions
int _num_conv;


// ADC methods
// Private methods

/**
 * @brief Enable clocks necessary for ADC conversions and the DMA
 * controller
 */
void adc_enable_clocks(void) {
  /* GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

  /* Configure the ADC clock */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
  // RCC_ADCCLKConfig(RCC_ADC34PLLCLK_Div2);

  /* Enable DMA1 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable DMA2 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  /* ADC1 Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
  // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC34, ENABLE);

  // enable TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}


// Public methods

/**
 * @brief Initializes ADC and DMA
 * @details This method handles all the initialization of the ADC
 * and DMA controllers. It also will register the necessary
 * interrupt routines and perform the initial ADC calibration.
 * The individual channels for conversions still have to be enabled
 * seperately.
 */
void adc_init(void) {
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  __IO uint16_t calibration_value = 0;

  // enable clocks
  adc_enable_clocks();

  // Enable DMAs
  // Enable DMA for ADC1
  // DMA_DeInit(DMA1_Channel1);
  DMA_InitTypeDef DMA_InitStructure;
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_BufferSize = 3;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data_buf1[0];
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  // Enable DMA1 Channel Transfer Complete interrupt
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

  //Enable DMA1 channel IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_Cmd(DMA1_Channel1, ENABLE);

  // Enable DMA for ADC2
  // DMA_DeInit(DMA2_Channel1);
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data_buf2[0];
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  DMA_Init(DMA2_Channel1, &DMA_InitStructure);

  // Enable DMA2 Channel Transfer Complete interrupt
  DMA_ITConfig(DMA2_Channel1, DMA_IT_TC, ENABLE);

  //Enable DMA1 channel IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_Cmd(DMA2_Channel1, ENABLE);


  // Enable ADCs
  ADC_StructInit(&ADC_InitStructure);

  /* Calibration procedure */
  // ADC1
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);

  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1) != RESET);
  calibration_value_adc1 = ADC_GetCalibrationValue(ADC1);

  // ADC2
  ADC_VoltageRegulatorCmd(ADC2, ENABLE);

  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC2);

  while(ADC_GetCalibrationStatus(ADC2) != RESET);
  calibration_value_adc2 = ADC_GetCalibrationValue(ADC2);


  // //ADC3
  // ADC_VoltageRegulatorCmd(ADC3, ENABLE);

  // ADC_SelectCalibrationMode(ADC3, ADC_CalibrationMode_Single);
  // ADC_StartCalibration(ADC3);

  // while(ADC_GetCalibrationStatus(ADC3) != RESET);
  // calibration_value_adc3 = ADC_GetCalibrationValue(ADC3);

  // //ADC4
  // ADC_VoltageRegulatorCmd(ADC4, ENABLE);

  // ADC_SelectCalibrationMode(ADC4, ADC_CalibrationMode_Single);
  // ADC_StartCalibration(ADC4);

  // while(ADC_GetCalibrationStatus(ADC4) != RESET);
  // calibration_value_adc4 = ADC_GetCalibrationValue(ADC4);

  /* Configure ADC1, ADC2, ADC3, and ADC4 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC2, &ADC_CommonInitStructure);
  // ADC_CommonInit(ADC3, &ADC_CommonInitStructure);
  // ADC_CommonInit(ADC4, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_11;
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel = 3;

  // Initialize ADCs
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  // ADC_Init(ADC3, &ADC_InitStructure);
  // ADC_Init(ADC4, &ADC_InitStructure);

  // Set number of enabled ADC channels
  num_chan_adc1 = 0;
  num_chan_adc2 = 0;
  // num_chan_adc3 = 0;
  // num_chan_adc4 = 0;
  
  // Set adc conversion state
  adc_state = 0x0;
  adc_val_rdy_state = 0x0;

  // Enable DMA mode for ADC
  ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC1, ENABLE);

  ADC_DMAConfig(ADC2, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC2, ENABLE);
}


/**
 * @brief Deinitialize ADCs
 */
void adc_deinit(void) {
  adc_stop();

  //reset ADCs
  ADC_DeInit(ADC1);
  ADC_DeInit(ADC2);
  // ADC_DeInit(ADC3);
  // ADC_DeInit(ADC4);

  num_chan_adc1 = 0;
  num_chan_adc2 = 0;
  // num_chan_adc3 = 0;
  // num_chan_adc4 = 0;
}

/**
 * @brief Start ADC conversions
 */
void adc_start(void) {
  /* Enable ADC */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  // ADC_Cmd(ADC3, ENABLE);
  // ADC_Cmd(ADC4, ENABLE);

  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));
  // while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_RDY));
  // while(!ADC_GetFlagStatus(ADC4, ADC_FLAG_RDY));

  /* ADC Start Conversion */
   ADC_StartConversion(ADC1);
   ADC_StartConversion(ADC2);
   // ADC_StartConversion(ADC3);
   // ADC_StartConversion(ADC4);
}

/**
 * @brief Stop ADC conversions
 */
void adc_stop(void) {
  /* Disable ADC1 and ADC2 */
  ADC_StopConversion(ADC1);
  ADC_StopConversion(ADC2);
  // ADC_StopConversion(ADC3);
  // ADC_StopConversion(ADC4);

  ADC_Cmd(ADC1, DISABLE);
  ADC_Cmd(ADC2, DISABLE);
  // ADC_Cmd(ADC3, DISABLE);
  // ADC_Cmd(ADC4, DISABLE);
}


/**
 * @brief Enable ADC channels to convert and set order
 * @detail Marks an ADC channel as enabled and sets the order
 * for the ADC conversions to be stored. This order roughly
 * corresponds to the actual order of conversions because they
 * are split based on what ADC they are performed on.
 * 
 * @param channels Array containing the ADC channels to convert in order
 * @param num_conv Number of total ADC channels enabled
 */
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


/**
 * @brief Initialize ADC channels
 * @details Handles the actual initialization of the external
 * ADC pins and enabling the correct ADC channel. This method
 * assumes that you have already called adc_enable_channels
 */
void adc_initialize_channels() {
  for (int k = 0; k < GE_NUM_ADC; k++) {
    if (adc_conv_order[k].enabled) {
      GPIOPin pin = adc_conv_order[k].pin;

      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = _ge_pin_num[pin];
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(_ge_pin_port[pin], &GPIO_InitStruct);

      if (adc_conv_order[k].STM_ADCx == ADC1) {
        num_chan_adc1++;

        // store index to save result
        adc1_conv_map[num_chan_adc1-1] = k;

        // mark that ADC1 is used
        // adc_val_rdy_state |= 0x1;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc1, ADC_SampleTime_19Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC2) {
        num_chan_adc2++;

        // mark that ADC1 is used
        // adc_val_rdy_state |= 0x2;

        // store index to save result
        adc2_conv_map[num_chan_adc2-1] = k;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc2, ADC_SampleTime_19Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC3) {
        num_chan_adc3++;

        // mark that ADC1 is used
        // adc_val_rdy_state |= 0x4;

        // store index to save result
        adc3_conv_map[num_chan_adc3-1] = k;
        
        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc3, ADC_SampleTime_19Cycles5);
      } else if (adc_conv_order[k].STM_ADCx == ADC4) {
        num_chan_adc4++;

        // mark that ADC1 is used
        // adc_val_rdy_state |= 0x1;

        // store index to save result
        adc4_conv_map[num_chan_adc4-1] = k;

        ADC_RegularChannelConfig(adc_conv_order[k].STM_ADCx, adc_conv_order[k].STM_ADC_chan, num_chan_adc4, ADC_SampleTime_19Cycles5);
      } else {
        return;
      }
    }
  }

  // enable conversion sequences for ADCs
  ADC_RegularChannelSequencerLengthConfig(ADC1, num_chan_adc1);
  ADC_RegularChannelSequencerLengthConfig(ADC2, num_chan_adc2);
  // ADC_RegularChannelSequencerLengthConfig(ADC3, num_chan_adc3);
  // ADC_RegularChannelSequencerLengthConfig(ADC4, num_chan_adc4);

  // setup DMAs

  DMA_Cmd(DMA1_Channel1, DISABLE);
  DMA_Cmd(DMA1_Channel2, DISABLE);

  // DMA for ADC1
  DMA_InitTypeDef DMA_InitStructure;
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_BufferSize = num_chan_adc1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data_buf1[0];
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  DMA_Init(DMA1_Channel1, &DMA_InitStructure);


  // DMA for ADC2
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_BufferSize = num_chan_adc2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data_buf2[0];
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  DMA_Init(DMA2_Channel1, &DMA_InitStructure);

  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_Cmd(DMA1_Channel2, ENABLE);

  // enable ADCs
  // ADC_Cmd(ADC1, ENABLE);
  // ADC_Cmd(ADC2, ENABLE);
  // ADC_Cmd(ADC3, ENABLE);
  // ADC_Cmd(ADC4, ENABLE);

  // DMA_Cmd(DMA1_Channel1, DISABLE);
  // DMA_Cmd(DMA1_Channel2, DISABLE);

  adc_state = 0x0;
}

/**
 * @brief Register callback function
 * @details Registers the provided callback function for when
 * the ADCs finish their conversion sequences. The callback
 * function must take an array of the conversion results as its
 * argument. The order of results is the same as the array
 * of enabled ADC channels.
 * 
 * @param callback Callback function that takes the conversion
 * results as its argument.
 */
void adc_callback(void (*callback)(uint16_t *)) {
  adc_reg_callback = callback;
}


/**
 * @brief Set the ADC sampling frequency
 * @details Sets the ADC sampling frequency to the specified
 * rate in Hz. The library uses Timer 2 to handle triggering
 * the ADC conversions. The ADC maximum sampling rate is 5 MHz
 * and the minimum for the library is 1.1 kHz
 * 
 * @param fs Sampling rate in Hz.
 */
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
}

/**
 * @brief Get the ADC conversion state
 * @return ADC conversion state. The state is represented as a
 * 4 bit value where the LSB is ADC1 and MSB is ADC4
 */
uint8_t adc_get_state() {
  return adc_state;
}


// Interrupt Handlers

/**
 * @brief Interrupt Handler for DMA1_CH1 - ADC1 DMA handler
 * @details Moves the results from data_buf1 to the correct
 * location in adc_conversions
 */
void DMA1_Channel1_IRQHandler(void) {
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA1_IT_TC1)) {
    // status=1;
    //Clear DMA1 interrupt pending bits
    DMA_ClearITPendingBit(DMA1_IT_GL1);
    DMA_ClearITPendingBit(DMA1_IT_TC1);

    // callback to handle ADC1 results
    for (int k = 0; k < num_chan_adc1; k++) {
      adc_conversions[adc1_conv_map[k]] = (uint16_t)(data_buf1[k]);
    }

    adc_state |= 0x1;

    // if all ADCs finished trigger callback
    if (adc_state == 0x3) {
      adc_state = 0x0;
      adc_reg_callback(adc_conversions);
    }

    // DMA_SetCurrDataCounter(DMA1_Channel1, num_chan_adc1);
  }
  // ADC_StartConversion(ADC1);
}

/**
 * @brief Interrupt Handler for DMA2_CH1 - ADC2 DMA handler
 * @details Moves the results from data_buf2 to the correct
 * location in adc_conversions
 */
void DMA2_Channel1_IRQHandler(void) {
  //Test on DMA2 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA2_IT_TC1)) {
    // status=1;
    //Clear DMA2 interrupt pending bits
    DMA_ClearITPendingBit(DMA2_IT_GL1);
    DMA_ClearITPendingBit(DMA2_IT_TC1);

    // callback to handle ADC2 results
    for (int k = 0; k < num_chan_adc2; k++) {
      adc_conversions[adc2_conv_map[k]] = (uint16_t)(data_buf2[k]);
    }

    adc_state |= 0x2;

    // if all ADCs finished trigger callback
    if (adc_state == 0x3) {
      adc_state = 0x0;
      adc_reg_callback(adc_conversions);
    }

    // DMA_SetCurrDataCounter(DMA2_Channel1, num_chan_adc2);
  }
  // ADC_StartConversion(ADC1);
}
