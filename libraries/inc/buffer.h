/**
 * @file buffer.h
 * @brief This file provides helper functions for managing
 * circular buffers for the interrupt UART library. Taken from
 * http://www.embedds.com/programming-stm32-usart-using-gcc-tools-part-1/
 * 
 * @author Ned Danyliw
 * @date 9.2016
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f30x.h"
#define USARTBUFFSIZE 128

typedef struct{
  uint8_t in;
  uint8_t out;
  uint8_t count;
  uint8_t buff[USARTBUFFSIZE];
} FIFO_TypeDef;

void BufferInit(__IO FIFO_TypeDef *buffer);
ErrorStatus BufferPut(__IO FIFO_TypeDef *buffer, uint8_t ch);
ErrorStatus BufferGet(__IO FIFO_TypeDef *buffer, uint8_t *ch);
ErrorStatus BufferIsEmpty(__IO FIFO_TypeDef buffer);

#ifdef __cplusplus
 }
#endif

#endif /* BUFFER_H_ */