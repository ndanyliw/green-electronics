/**
 * @file  ge_eeprom.h
 * @brief Wrapper library for emulated EEPROM on STM32F3
 * 
 * @details Wraps around the EEPROM emulation example code
 * provided by STMicro and found in eeprom.h/c.
 * http://www.st.com/web/en/catalog/tools/PF258148
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_EEPROM
#define _GE_EEPROM

#ifdef __cplusplus
 extern "C" {
#endif


#include "eeprom.h"

//Virtual addresses
extern uint16_t VirtAddVarTab[NB_OF_VAR];

void eeprom_init();
uint16_t eeprom_read(uint16_t addr, uint16_t* data);
uint16_t eeprom_write(uint16_t addr, uint16_t data);



#ifdef __cplusplus
 }
#endif

#endif