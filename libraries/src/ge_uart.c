/**
 * @file  ge_uart.c
 * 
 * @brief UART library for the STM32 Discovery board for use with the
 * EE155 breakout board.
 * 
 * Code adapted from http://www.embedds.com/programming-stm32-usart-using-gcc-tools-part-1/
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */


#include "ge_uart.h"

#ifdef BUFFERED
//initialize buffers
volatile FIFO_TypeDef U1Rx, U1Tx;
#endif


void ge_uart_init(uint32_t baud) {
  #ifdef BUFFERED
  BufferInit(&U1Rx);
  BufferInit(&U1Tx);
  #endif

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  USART_ClockInitTypeDef USART_ClockInitStructure;
  //enable bus clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //Set USART1 Tx (PC.4) as AF push-pull
  //Set USART1 Rx (PC.5) as input floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // set alternate functions
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
  
  USART_StructInit(&USART_InitStructure);
  USART_ClockStructInit(&USART_ClockInitStructure);
  USART_ClockInit(USART1, &USART_ClockInitStructure);
  
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  //Write USART1 parameters
  USART_Init(USART1, &USART_InitStructure);

  //Enable USART1
  USART_Cmd(USART1, ENABLE);

  #ifdef BUFFERED
  //configure NVIC
  NVIC_InitTypeDef NVIC_InitStructure;
  //select NVIC channel to configure
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  //set priority to lowest
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //set subpriority to lowest
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //enable IRQ channel
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //update NVIC registers
  NVIC_Init(&NVIC_InitStructure);
  //disable Transmit Data Register empty interrupt
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  //enable Receive Data register not empty interrupt
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  // NVIC_EnableIRQ(USART1_IRQn);
  #endif
}

void ge_uart_put(uint8_t ch) {
  #ifdef BUFFERED
    //put char to the buffer
    BufferPut(&U1Tx, ch);
    //enable Transmit Data Register empty interrupt
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  #else
    USART_SendData(USART1, (uint8_t) ch);
    //Loop until the end of transmission
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
    }
  #endif
}

uint8_t ge_uart_get(void) {
  #ifdef BUFFERED
    uint8_t ch;
    //check if buffer is empty
    while (BufferIsEmpty(U1Rx) ==SUCCESS);
    BufferGet(&U1Rx, &ch);
    return ch;
  #else
     while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
        return (uint8_t)USART_ReceiveData(USART1);
  #endif
}

int ge_uart_available() {
  return !(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
}

void ge_uart_write(char *str, int len) {
  for (int i = 0; i < len; i++) {
    ge_uart_put(str[i]);
  }
}

// int _write(int file, char *ptr, int len) {
//     int n;
//     switch (file) {
//     case STDOUT_FILENO: /*stdout*/
//         for (n = 0; n < len; n++) {
//             Usart1Put(*ptr++ & (uint16_t)0x01FF);
//         }
//         break;
//     case STDERR_FILENO: /* stderr */
//         for (n = 0; n < len; n++) {
//             Usart1Put(*ptr++ & (uint16_t)0x01FF);
//         }
//         break;
//     default:
//         errno = EBADF;
//         return -1;
//     }
//     return len;
// }

// int _read(int file, char *ptr, int len) {
//     int n;
//     int num = 0;
//     switch (file) {
//     case STDIN_FILENO:
//         for (n = 0; n < len; n++) {
//             char c = Usart1Get();
//             *ptr++ = c;
//             num++;
//         }
//         break;
//     default:
//         errno = EBADF;
//         return -1;
//     }
//     return num;
// }

void USART1_IRQHandler(void)
{
  uint8_t ch;
  //if Receive interrupt
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    ch=(uint8_t)USART_ReceiveData(USART1);
  #ifdef BUFFERED
      //put char to the buffer
      BufferPut(&U1Rx, ch);
  #endif
  }
  if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
  #ifdef BUFFERED
      if (BufferGet(&U1Tx, &ch) == SUCCESS)//if buffer read
      {
        USART_SendData(USART1, ch);
      }
      else//if buffer empty
  #endif
      {
        //disable Transmit Data Register empty interrupt
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      }
  }
}