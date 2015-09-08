/*
 * HD44780_F3.h
 *
 *  Created on: Sep 26, 2014
 *      Author: alexanderdewing
 */

#ifndef SRC_HD44780_F3_H_
#define SRC_HD44780_F3_H_

#ifdef __cplusplus
 extern "C" {
#endif



void hd44780_Delay(unsigned long del);
void hd44780_wr_hi_nibble(unsigned char data);
void hd44780_wr_lo_nibble(unsigned char data);
void hd44780_write(unsigned char data);
void hd44780_wr_cmd(unsigned char cmd);
void hd44780_wr_data(unsigned char data);

void HD44780_Init(void);
void HD44780_PutChar(unsigned char c);
void HD44780_GotoXY(unsigned char x, unsigned char y);
void HD44780_PutStr(char *str);
void HD44780_ClrScr(void);

#ifdef __cplusplus
 }
#endif

#endif /* SRC_HD44780_F3_H_ */
