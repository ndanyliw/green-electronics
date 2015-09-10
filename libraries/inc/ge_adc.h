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
#include "cortexm/ExceptionHandlers.h"

//definitions
//ADC Channels
#define ADC1_CHAN1 1
#define ADC1_CHAN2 2
#define ADC1_CHAN3 3
#define ADC1_CHAN4 4
#define ADC1_CHAN5 5  // not present in discovery board
#define ADC12_CHAN6 6
#define ADC12_CHAN7 7
#define ADC12_CHAN8 8
#define ADC12_CHAN9 9
#define ADC12_CHAN10 10  // not present in discovery board
#define ADC1_CHAN11 11
#define ADC1_CHAN12 12
#define ADC1_CHAN13 13
#define ADC2_CHAN1 14
#define ADC2_CHAN2 15
#define ADC2_CHAN3 16
#define ADC2_CHAN4 17
#define ADC2_CHAN5 18
#define ADC2_CHAN11 19
#define ADC2_CHAN12 20
#define ADC2_CHAN13 21
#define ADC2_CHAN14 22
#define ADC2_CHAN15 23

// physical pin mapping of ADC channels
extern int adc_pin_map[23];

extern GPIO_TypeDef *adc_bank_map[23];

//saved data buffer
uint32_t data_buf[15];

//conversion order
uint16_t chan_order[15];

//associated callbacks
void (*adc_reg_callbacks[15])(uint32_t);

//initialize ADC settings
void adc_init(void);
void adc_deinit(void); //resets all ADC settings

//startup ADCs
void adc_start(void);
void adc_stop(void);

//enable channels
void adc_enable_channel(uint16_t chan);
//void adc_disable_channel(uint16_t chan, uint16_t rank);

////set channel priority
//void adc_set_chan_rank(uint16_t chan, uint16_t rank);

////calibrate ADCs
//void adc_selfcal();

//set sampling frequency
void adc_set_fs(float fs);

//attach callback function to a specific channel
void adc_callback(const int chan, void (*callback)(uint32_t));

#ifdef __cplusplus
}
#endif

#endif /* GE_ADC_H_ */
