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


/**
 * @brief Initialize serial library
 * @details Sets PC4 and PC5 as TX and RX pins respectively.
 * The library uses the USART1 module and handles its
 * initialization as well as the necessary GPIO init structures
 * and interrupts.
 * 
 * @param baud Baud rate for serial communication
 */
void ge_uart_init(uint32_t baud) {
  #ifdef BUFFERED
  // initialize circular FIFO buffers
  BufferInit(&U1Rx);
  BufferInit(&U1Tx);
  #endif

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  USART_ClockInitTypeDef USART_ClockInitStructure;
  //enable bus clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  // Initialize GPIO
  //Set USART1 Tx (PC.4) as AF push-pull
  //Set USART1 Rx (PC.5) as input floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // set alternate functions
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);
  
  // Initialize USART1
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


/**
 * @brief Send a character over the serial port
 * @details Sends a single character over the serial port. This
 * method will block if it is not buffered.
 * 
 * @param ch Character to send
 */
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


/**
 * @brief Returns the recieved character from the serial port
 * @details Returns the recieved character from the serial port.
 * This method will block if there is no character in the RX buffer
 * or wait until a character is recieved if no buffer is used.
 * @return Character recieved
 */
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


/**
 * @brief Checks if there is a character waiting to be recieved
 * @details Checks if there is a character waiting to be recieved.
 * In the buffered mode, this checks if the buffer is not empty. In
 * the non-buffered mode, the method checks if the USART has
 * recieved a character
 * @return 1 if character available.
 */
int ge_uart_available() {
  #ifdef BUFFERED
    return !(BufferIsEmpty(U1Rx));
  #else
    return !(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  #endif
}


/**
 * @brief Write a string to the serial port
 * @details Sends each character individually over the serial port
 * 
 * @param str String to send
 * @param len Length of string
 */
void ge_uart_write(char *str, int len) {
  for (int i = 0; i < len; i++) {
    ge_uart_put(str[i]);
  }
}


/**
 * @brief USART1 handler
 * @details Interrupt handler stores recieved data in RX buffer
 * and sends data from TX buffer if it is available.
 */
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
