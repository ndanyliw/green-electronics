/*
 * ge_adc.h

 *
 *  Created on: Oct 3, 2014
 *      Author: Ned
 */

#ifndef GE_ADC_H_
#define GE_ADC_H_

#ifdef __cplusplus
 extern "C" {
#endif

//Perform necessary includes
#include "common.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x.h"
#include "stm32f30x_dma.h"
#include "stm32f30x_tim.h"
#include "ge_pins.h"
// #include "cortexm/ExceptionHandlers.h"

//definitions
/**
 * Enumerate ADC channels mapped to external pins on the STM32 Discovery
 */
#define NUM_ADC 39



typedef enum ADC_CHAN
{ 
  GE_ADC1_1, GE_ADC1_2, GE_ADC1_3, GE_ADC1_4, GE_ADC1_5,
  GE_ADC2_1, GE_ADC2_2, GE_ADC2_3, GE_ADC2_4, GE_ADC2_5,
  GE_ADC12_6, GE_ADC12_7, GE_ADC12_8, GE_ADC12_9, GE_ADC12_10,
  GE_ADC2_11, GE_ADC2_12, 
  GE_ADC3_1, GE_ADC3_2, GE_ADC3_3, GE_ADC3_5, GE_ADC3_12, GE_ADC3_13, GE_ADC3_14,
  GE_ADC3_15, GE_ADC3_16,
  GE_ADC34_6, GE_ADC34_7, GE_ADC34_8, GE_ADC34_9, GE_ADC34_10, GE_ADC34_11,
  GE_ADC4_1, GE_ADC4_2, GE_ADC4_3, GE_ADC4_4, GE_ADC4_5, GE_ADC4_12, GE_ADC4_13 
} ADC_CHAN_Type;

// physical pin mapping of ADC channels
extern GPIOPin adc_pin_map[NUM_ADC];

extern ADC_TypeDef *adc_bank_map[NUM_ADC];
extern uint8_t adc_chan_map[NUM_ADC];

// structure to keep track of ADC channel parameters
typedef struct
{
  GPIOPin pin;
  ADC_CHAN_Type chan;
  ADC_TypeDef *STM_ADCx;
  uint8_t STM_ADC_chan;
  bool enabled;
  uint16_t period;
} GE_ADC_chan_info;

GE_ADC_chan_info adc_conv_order[NUM_ADC];

// ADC_CHAN_Type adc_conv_order[NUM_ADC];

//saved data buffer
uint32_t data_buf[16];

//conversion order
int chan_order1[16];
int chan_order2[16];
int chan_order3[16];
int chan_order4[16];

//number of channels per ADC
uint8_t num_chan_adc1;
uint8_t num_chan_adc2;
uint8_t num_chan_adc3;
uint8_t num_chan_adc4;

//associated callbacks
void (*adc_reg_callbacks1[16])(uint32_t);
void (*adc_reg_callbacks2[16])(uint32_t);
void (*adc_reg_callbacks3[16])(uint32_t);
void (*adc_reg_callbacks4[16])(uint32_t);

//registered callback
void (*adc_reg_callback)(uint16_t *, uint16_t *, uint16_t *, uint16_t *);

//initialize ADC settings
void adc_init(void);
void adc_deinit(void); //resets all ADC settings

//startup ADCs
void adc_start(void);
void adc_stop(void);

//enable channels
void adc_enable_channel(uint16_t chan);
//void adc_disable_channel(uint16_t chan, uint16_t rank);

void adc_enable_channels(ADC_CHAN_Type *channels, uint16_t num_conv);
void adc_initialize_channels();

////set channel priority
//void adc_set_chan_rank(uint16_t chan, uint16_t rank);

////calibrate ADCs
//void adc_selfcal();

//set sampling frequency
void adc_set_fs(float fs);

//attach callback function to a specific channel
void adc_callback(void (*callback)(uint16_t *, uint16_t *, uint16_t *, uint16_t *));

#ifdef __cplusplus
}
#endif

#endif /* GE_ADC_H_ */
