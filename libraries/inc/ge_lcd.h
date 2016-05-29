/**
 * @file  ge_lcd.h
 * @brief EE155 library for interfacing with the class LCD.
 * 
 * Provides a basic interface for the LCD. The LCD uses a HD44780
 * protocol and this library wraps around a prewritten HD44780
 * library.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#ifndef _GE_LCD_H
#define _GE_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

//Includes
#include "common.h"
#include "HD44780_F3.h"

void lcd_init(void);
void lcd_putc(unsigned char c);
void lcd_goto(uint8_t x, uint8_t y);
void lcd_puts(char *str);
void lcd_clear(void);


#ifdef __cplusplus
}
#endif

#endif