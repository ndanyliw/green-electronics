/**
 * @file  ee155_lcd.c
 * @brief Source code for EE155 LCD library
 * 
 * @author Ned
 * @date  09.2015
 */

 #include "ee152_lcd.h"


/**
 * @brief Initializes the LCD
 * @details Initializes necessary IO and sets up the display
 */
void lcd_init(void) {
  HD44780_Init();
}


/**
 * @brief Puts a character in the current cursor position
 * @details Place a character on the LCD at the current position
 * 
 * @param char Character to display
 */
void lcd_putc(unsigned char c) {
  HD44780_PutChar(c);
}


/**
 * @brief Goto a specific LCD position
 * @details Sets the cursor's X and Y position on the screen
 * 
 * @param x X position
 * @param y Y position
 */
void lcd_goto(uint8_t x, uint8_t y) {
  HD44780_GotoXY((unsigned char) x, (unsigned char) y);
}


/**
 * @brief Print a string to the LCD
 * @details Prints a string to the LCD starting at the current cursor
 * position
 * 
 * @param str String to print
 */
void lcd_puts(char *str) {
  HD44780_PutStr(str);
}


/**
 * @brief Clear the display
 * @details Clear the LCD display
 */
void lcd_clear(void) {
  HD44780_ClrScr();
}
