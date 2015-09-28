/**
 * @file  ge_eeprom.c
 * @brief Implementation of virtual EEPROM wrapper library
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "ge_eeprom.h"
#include "stm32f30x_flash.h"


/**
 * @brief Initialize EEPROM
 * @details Sets aside FLASH memory and prepares for EEPROM
 * emulation
 */
void eeprom_init() {
  //Unlock flash program erase controller
  FLASH_Unlock();
  //Initialize EEPROM
  EE_Init();
}


/**
 * @brief Reads from the specified virtual address
 * @details Reads the data stored at the virtual address
 * and stores it in the provided data structure
 * 
 * @param addr Address to read from
 * @param data Variable to store the data in
 * 
 * @return Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
 */
uint16_t eeprom_read(uint16_t addr, uint16_t *data) {
  return EE_ReadVariable(addr, data);
}


/**
 * @brief Writes to the specified virtual address
 * @details Writes the specified data at the provided virtual
 * address.
 * 
 * @param addr Address to write to
 * @param data Data to save
 * 
 * @return Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
 */
uint16_t eeprom_write(uint16_t addr, uint16_t data) {
  return EE_WriteVariable(addr, data);
}