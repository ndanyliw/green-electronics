/**
 * @file  ge_vcom.h
 * @brief Creates a virtual COM port using the user USB port on
 * the Discovery board.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_VCOM_H
#define _GE_VCOM_H
 
#ifdef __cplusplus
 extern "C" {
#endif


//Includes
#include "common.h"
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "stm32f30x_conf.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "platform_config.h"
#include "math.h"
#include "usb_istr.h"


//Private variables
//Only define if compiling library
#ifdef GE_LIB
/** Position of buffer head (what the user would read) */
__IO uint8_t _vcom_buf_head = 0;
/** Position of the buffer tail (where to store the next received char) */
__IO uint8_t _vcom_buf_tail = 0;
/** Flag that is set if the buffer overflows */
__IO uint8_t _vcom_buf_corrupt = 0;
/** 256 character buffer */
__IO char _vcom_buf[256];
/** Number of available bytes in the buffer */
__IO uint16_t _vcom_buf_available;
/** Packet sent flag */
uint32_t packet_sent=1;
#endif

extern  __IO uint32_t bDeviceState; /* USB device status */
extern  __IO uint32_t _ge_usb_timeout;

//Exported functions
void vcom_init();
void vcom_send(char *data);
uint16_t vcom_available();
char vcom_read();
bool vcom_corrupt();
void vcom_reset();

//Interrupt handlers
void USB_LP_CAN1_RX0_IRQHandler(void);
void USB_LP_IRQHandler(void);
void USBWakeUp_IRQHandler(void);
void USBWakeUp_RMP_IRQHandler(void);

#ifdef __cplusplus
 }
#endif

#endif