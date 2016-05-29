/**
 * @file  ge_uart.h
 * 
 * @brief UART library for the STM32 Discovery board for use with the
 * EE155 breakout board
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_UART_H
#define _GE_UART_H

#ifdef __cplusplus
  extern "C" {
#endif

// includes
#include "common.h"
#include "stm32f30x_usart.h"


void ge_uart_init(uint32_t baud);
void ge_uart_put(uint8_t ch);
uint8_t ge_uart_get();
int ge_uart_available();



#ifdef __cplusplus
  }
#endif

#endif