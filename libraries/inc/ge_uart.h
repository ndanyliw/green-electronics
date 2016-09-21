/**
 * @file  ge_uart.h
 * 
 * @brief UART library for the STM32 Discovery board for use with the
 * EE155 breakout board.
 * 
 * Code adapted from http://www.embedds.com/programming-stm32-usart-using-gcc-tools-part-1/
 * 
 * @author Ned Danyliw
 * @date  08.2016
 */

#ifndef _GE_UART_H
#define _GE_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"
#include "ge_pins.h"
#include "buffer.h"

// Buffered mode uses interrupts to handle communication
#define BUFFERED
#ifdef BUFFERED
extern volatile FIFO_TypeDef U1Rx, U1Tx;
#endif

void ge_uart_init(uint32_t baud);
void ge_uart_put(uint8_t ch);
uint8_t ge_uart_get(void);
int ge_uart_available();
void ge_uart_write(char *str, int len);

#ifdef __cplusplus
 }
#endif

#endif