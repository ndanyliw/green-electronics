/**
 * @file  ge_vcom.c
 * @brief Implementation of virtual COM port for the green
 * electronics library
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "ge_vcom.h"

#include <string.h>

/**
 * @brief Initialize the VCOM interface
 * @details Enables the USB device on the STM board and configures
 * it as a virtual COM port.
 */
void vcom_init() {
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();

  USB_Init();

  _ge_usb_timeout = 100;

  while ((bDeviceState != CONFIGURED) && (_ge_usb_timeout != 0))
  {}

  //initialize positions
  _vcom_buf_head = 0;
  _vcom_buf_tail = 0;
  _vcom_buf_corrupt = 0;

  //enable receive channel
  CDC_Receive_DATA();
}

/**
 * @brief Send a string over the VCOM port
 * @details Sends a string over the VCOM port. The string
 * must be less than 255 characters and terminated by '\0'
 * 
 * @param data String to send
 */
void vcom_send(char *data) {
  //calculate string length
  uint8_t str_len = strlen(data);

  //send data
  CDC_Send_DATA((uint8_t *)data, str_len);
}

/**
 * @brief Read a character from the virtual COM port
 * @details Returns the latest character in the receive buffer
 * and removes it from the buffer.
 * @return The top character in the buffer.
 */
char vcom_read() {
  //check if any data available and return 0 if not
  if(!_vcom_buf_available--) return 0;

  return _vcom_buf[_vcom_buf_head++];
}

/**
 * @brief Checks if the buffer has been corrupted.
 * @details Checks the buffer corrupt flag and returns its state.
 * To clear the flag, call @ref vcom_reset
 * @return True if corrupt.
 */
bool vcom_corrupt() {
  return _vcom_buf_corrupt == 1;
}

/**
 * @brief Resets the receive buffer
 * @details Resets the receive buffer. Any data inside will be lost.
 */
void vcom_reset() {
  _vcom_buf_head = 0;
  _vcom_buf_tail = 0;
  _vcom_buf_available = 0;
  _vcom_buf_corrupt = 0;
}

/**
 * @brief Return the number of bytes available to read.
 * @details Returns the number of available bytes. Make sure to
 * check this before calling vcom_read()
 * @return Number of available bytes
 */
uint16_t vcom_available() {
  return (uint8_t) _vcom_buf_available;
}

//Interrupt handlers
#if defined (USB_INT_DEFAULT)
void USB_LP_CAN1_RX0_IRQHandler(void)
#elif defined (USB_INT_REMAP)
void USB_LP_IRQHandler(void)
#endif
{
   USB_Istr();
}

#if defined (USB_INT_DEFAULT)
void USBWakeUp_IRQHandler(void)
#elif defined (USB_INT_REMAP)
void USBWakeUp_RMP_IRQHandler(void)
#endif
{
  /* Initiate external resume sequence (1 step) */
  Resume(RESUME_EXTERNAL);  
  EXTI_ClearITPendingBit(EXTI_Line18);
}

