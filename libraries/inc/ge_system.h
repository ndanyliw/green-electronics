/**
 * @file  ge_system.h
 * @brief System level functions to use with the green electonics
 * libraries.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_SYSTEM_H
#define _GE_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"

//system ticks per second (1 ms period)
#define GE_TICKS_S  1000

void ge_init();
void delay_ms(__IO uint32_t ms);


#ifdef __cplusplus
  }
#endif

#endif