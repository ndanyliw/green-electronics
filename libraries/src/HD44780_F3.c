/*#############################################################
Driver name	    : HD44780_F3.c
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)
Tested On       : STM32F3-Discovery

Description			: Provides a library to access a HD44780-based
									character LCD module.

Requirements    : * STM32F3-Discovery Board
									* Define the custom configurations on line 
										80 to 112 in this file.

Functions				: HD44780_Init
									HD44780_ClrScr
									HD44780_GotoXY
									HD44780_PutStr
									HD44780_PutChar
													  
Special Note(s) : NONE
##############################################################*/

#include <stdio.h>
#include <math.h>
#include <stm32f30x_gpio.h>
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "common.h"

#include "HD44780_F3.h"

/***************************************************************
 *
 * CONSTANTS
 *
 ***************************************************************/

/* COMMANDS */
#define HD44780_CMD_RESET            	0x30     	/*!< Resets display - used in init 3x */
#define HD44780_CMD_CLEAR            	0x01     	/*!< Clears display */
#define HD44780_CMD_RETURN_HOME      	0x02     	/*!< Sets DDRAM pointer to 0 */
#define HD44780_CMD_ENTRY_MODE       	0x04     	/*!< Sets how the pointer is updated after a character write */
#define HD44780_CMD_DISPLAY          	0x08     	/*!< Display settings */
#define HD44780_CMD_SHIFT            	0x10     	/*!< Cursor and display movement */
#define HD44780_CMD_FUNCTION         	0x20     	/*!< Screen type setup */
#define HD44780_CMD_CGRAM_ADDR       	0x40     	/*!< Sets CGRAM address */
#define HD44780_CMD_DDRAM_ADDR       	0x80     	/*!< Sets DDRAM address */

/* ENTRY_MODE Command parameters */
#define HD44780_ENTRY_SHIFT_DISP 			0x01	 		/*!< Shift display */
#define HD44780_ENTRY_SHIFT_CURS 			0x00	 		/*!< Shift cursor */
#define HD44780_ENTRY_ADDR_INC   			0x02     	/*!< Increments pointer */
#define HD44780_ENTRY_ADDR_DEC   			0x00	 		/*!< Decrements pointer */

/* DISPLAY Command parameters */
#define HD44780_DISP_ON       				0x04      /*!< Enables the display */
#define HD44780_DISP_OFF      				0x00      /*!< Disables the display */
#define HD44780_DISP_CURS_ON  				0x02      /*!< Enables cursor */
#define HD44780_DISP_CURS_OFF 				0x00      /*!< Disables cursor */
#define HD44780_DISP_BLINK_ON					0x01      /*!< Enables cursor blinking */
#define HD44780_DISP_BLINK_OFF  			0x00      /*!< Disables cursor blinking */

/* SHIFT Command parameters */
#define HD44780_SHIFT_DISPLAY    			0x08      /*!< Shifts the display or shifts the cursor if not set */
#define HD44780_SHIFT_CURSOR    			0x00      /*!< Shifts the display or shifts the cursor if not set */
#define HD44780_SHIFT_RIGHT      			0x04      /*!< Shift to the right */
#define HD44780_SHIFT_LEFT      			0x00      /*!< Shift to the left  */

/* FUNCTION Command parameters */
#define HD44780_FUNC_BUS_8BIT  				0x10      /*!< 8 bit bus */
#define HD44780_FUNC_BUS_4BIT  				0x00      /*!< 4 bit bus */
#define HD44780_FUNC_LINES_2   				0x08      /*!< 2 lines */
#define HD44780_FUNC_LINES_1   				0x00      /*!< 1 line */
#define HD44780_FUNC_FONT_5x10 				0x04      /*!< 5x10 font */
#define HD44780_FUNC_FONT_5x8  				0x00      /*!< 5x8 font */

/* Busy Flag - actually not used */
#define HD44780_BUSY_FLAG        			0x80      /*!< Busy flag */


/***************************************************************
 *
 * CONFIGURATION
 *
 ***************************************************************/

#define HD44780_CONF_BUS							HD44780_FUNC_BUS_4BIT
#define HD44780_CONF_LINES						HD44780_FUNC_LINES_2
#define HD44780_CONF_FONT							HD44780_FUNC_FONT_5x8

#define HD44780_DISP_LENGTH						20
#define HD44780_DISP_ROWS							4
#define HD44780_CONF_SCROLL_MS				20

/* HD44780 Data lines - use the same port for all the lines */
#define HD44780_DATAPORT							GPIOD
#define HD44780_DATABIT0							GPIO_Pin_0	//not used in 4-bit mode
#define HD44780_DATABIT1							GPIO_Pin_1	//not used in 4-bit mode
#define HD44780_DATABIT2							GPIO_Pin_2	//not used in 4-bit mode
#define HD44780_DATABIT3							GPIO_Pin_3	//not used in 4-bit mode
#define HD44780_DATABIT4							GPIO_Pin_4
#define HD44780_DATABIT5							GPIO_Pin_5
#define HD44780_DATABIT6							GPIO_Pin_6
#define HD44780_DATABIT7							GPIO_Pin_7

/* HD44780 Control lines - use the same port for all the lines */
#define HD44780_CONTROLPORT						GPIOD
#define HD44780_RS_BIT								GPIO_Pin_0
#define HD44780_RW_BIT								GPIO_Pin_1
#define HD44780_EN_BIT								GPIO_Pin_2

/* defines the peripheral clocks that need to be enabled for the above pins */
#define HD44780_RCC_AHBPeriph					RCC_AHBPeriph_GPIOD		//or something like "RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOD"


/***************************************************************
 *
 * FUNCTIONS
 *
 ***************************************************************/

#define hd44780_RS_On()		            GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_RS_BIT, Bit_SET)
#define hd44780_RS_Off()	           	GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_RS_BIT, Bit_RESET)
#define hd44780_RW_On()		            GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_RW_BIT, Bit_SET)
#define hd44780_RW_Off()	            GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_RW_BIT, Bit_RESET)
#define hd44780_EN_On()		            GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_EN_BIT, Bit_SET)
#define hd44780_EN_Off()	            GPIO_WriteBit(HD44780_CONTROLPORT, HD44780_EN_BIT, Bit_RESET)

#define hd44780_EN_high_delay()     	delay_ms(2)//4095)
#define hd44780_init_delay()        	delay_ms(16)	//16ms
#define hd44780_init_delay2()       	delay_ms(5)	//5ms
#define hd44780_init_delay3()       	delay_ms(1)	//1ms
#define hd44780_init_end_delay()    	delay_ms(2)	//2ms

#define hd44780_clear()                       	  hd44780_wr_cmd( HD44780_CMD_CLEAR )
#define hd44780_home()                        	  hd44780_wr_cmd( HD44780_CMD_RETURN_HOME )
#define hd44780_entry( inc_dec, shift )           hd44780_wr_cmd( ( HD44780_CMD_ENTRY_MODE | inc_dec | shift ) & 0x07 )
#define hd44780_display( on_off, cursor, blink )  hd44780_wr_cmd( ( HD44780_CMD_DISPLAY | on_off | cursor | blink ) & 0x0F )
#define hd44780_shift( inc_dec, shift )           hd44780_wr_cmd( ( HD44780_CMD_SHIFT | inc_dec | shift ) & 0x1F )
#define hd44780_function( bus, lines, font )      hd44780_wr_cmd( ( HD44780_CMD_FUNCTION | bus | lines | font ) & 0x3F )
#define hd44780_cgram_addr( addr )                hd44780_wr_cmd( HD44780_CMD_CGRAM_ADDR | ( addr & 0x3F ) )
#define hd44780_ddram_addr( addr )                hd44780_wr_cmd( HD44780_CMD_DDRAM_ADDR | ( addr & 0x7F ) )
#define hd44780_write_char( c )                   hd44780_wr_data( c & 0xff )

/* Function used from the CooCox HD44780 library */
/********************************************************************************************/
void hd44780_wr_hi_nibble( unsigned char data )
{
	if ( data & 0x10 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	}
	if ( data & 0x20 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	}
	if ( data & 0x40 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	}
	if ( data & 0x80 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	}

    /* set the EN signal */
    hd44780_EN_On();

    /* wait */
    hd44780_EN_high_delay();

    /* reset the EN signal */
    hd44780_EN_Off();
}


#if HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT

void hd44780_wr_lo_nibble( unsigned char data )
{
	if ( data & 0x01 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	}
	if ( data & 0x02 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	}
	if ( data & 0x04 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	}
	if ( data & 0x08 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	}

    /* set the EN signal */
    hd44780_EN_On();

    /* wait */
    hd44780_EN_high_delay();

    /* reset the EN signal */
    hd44780_EN_Off();
}

/* 4bit bus version */
void hd44780_write( unsigned char data )
{
	/* send the data bits - high nibble first */
	hd44780_wr_hi_nibble( data );
	hd44780_wr_lo_nibble( data );
}
#endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT */


#if HD44780_CONF_BUS == HD44780_FUNC_BUS_8BIT

/* 8bit bus version */
void hd44780_write( unsigned char data )
{
	/* set the data bits */
	if ( data & 0x01 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT0 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT0 );
	}
	if ( data & 0x02 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT1 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT1 );
	}
	if ( data & 0x04 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT2 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT2 );
	}
	if ( data & 0x08 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT3 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT3 );
	}
	if ( data & 0x10 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	}
	if ( data & 0x20 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	}
	if ( data & 0x40 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	}
	if ( data & 0x80 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	}

    /* tell the lcd that we have a command to read in */
    hd44780_EN_On();

    /* wait long enough so that the lcd can see the command */
    hd44780_EN_high_delay();

    /* reset the ce line */
    hd44780_EN_Off();
    hd44780_init_end_delay();

}
#endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_8BIT */


void hd44780_wr_cmd( unsigned char cmd )
{
	hd44780_RS_Off();
	hd44780_write( cmd );
}


void hd44780_wr_data( unsigned char data )
{
	hd44780_RS_On();
	hd44780_write( data );
}
/********************************************************************************************/



/*********************************************************************************************
Function name   : hd44780_Delay
Author 					: Grant Phillips
Date Modified   : 25/08/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Creates a delay which is simply a code loop which is independent from any
									hardware timers.

Special Note(s) : NONE

Parameters			: del		-	32-bit value to represent the delay cycles
Return value		: NONE
*********************************************************************************************/
void hd44780_Delay(unsigned long del)
{
	unsigned long i=0;

	while(i<del)
		i++;
}




/*********************************************************************************************
Function name   : HD44780_Init
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Initializes the HD44780 lcd module

Special Note(s) : NONE

Parameters			: NONE
Return value		: NONE
*********************************************************************************************/
void HD44780_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	/* Configure the peripheral clocks for the HD44780 data and control lines */
	RCC_AHBPeriphClockCmd(HD44780_RCC_AHBPeriph, ENABLE);

	/* Configure the HD44780 Data lines (DB7 - DB4) as outputs*/
	GPIO_InitStructure.GPIO_Pin = HD44780_DATABIT7 | HD44780_DATABIT6 | HD44780_DATABIT5 | HD44780_DATABIT4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;									
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HD44780_DATAPORT, &GPIO_InitStructure);

	/* Configure the HD44780 Control lines (RS, RW, EN) as outputs*/
	GPIO_InitStructure.GPIO_Pin = HD44780_RS_BIT | HD44780_RW_BIT | HD44780_EN_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;									
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HD44780_CONTROLPORT, &GPIO_InitStructure);

  /* clear control bits */
	hd44780_EN_Off();
	hd44780_RS_Off();
	hd44780_RW_Off();
	
	/* wait initial delay for LCD to settle */
  /* reset procedure - 3 function calls resets the device */
  hd44780_init_delay();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );
  hd44780_init_delay2();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );
  hd44780_init_delay3();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );
		
	#if HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT
    /* 4bit interface */
  hd44780_wr_hi_nibble( HD44780_CMD_FUNCTION );
  #endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT */

  /* sets the configured values - can be set again only after reset */
  hd44780_function( HD44780_CONF_BUS, HD44780_CONF_LINES, HD44780_CONF_FONT );

  /* turn the display on with no cursor or blinking */
	hd44780_display( HD44780_DISP_ON, HD44780_DISP_CURS_OFF, HD44780_DISP_BLINK_OFF );
	
  /* clear the display */
  hd44780_clear();

  /* addr increment, shift cursor */
	hd44780_entry( HD44780_ENTRY_ADDR_INC, HD44780_ENTRY_SHIFT_CURS );

}



/*********************************************************************************************
Function name   : HD44780_PutChar
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Prints a character at the current character cursor position

Special Note(s) : NONE

Parameters			: c				-	character to print

Return value		: NONE
*********************************************************************************************/
void HD44780_PutChar(unsigned char c)
{
	hd44780_wr_data(c & 0xff);
}



/*********************************************************************************************
Function name   : HD44780_GotoXY
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Changes the position of the current character cursor from where the next 
									characters will be printed.

Special Note(s) : NONE

Parameters			: x				-	column position (0 - HD44780_DISP_LENGTH-1)
									y				-	row position (0 - HD44780_DISP_ROWS-1)

Return value		: NONE
*********************************************************************************************/
void HD44780_GotoXY(unsigned char x, unsigned char y)
{
  unsigned char copy_y=0;
	
  if(x > (HD44780_DISP_LENGTH-1))
		x = 0;

	if(y > (HD44780_DISP_ROWS-1))
		y = 0;

	switch(y)
  {
		case 0:  copy_y = 0x80; break;
		case 1:  copy_y = 0xc0; break;
		case 2:  copy_y = 0x94; break;
		case 3:  copy_y = 0xd4; break;
  }
	hd44780_wr_cmd(x + copy_y);
}



/*********************************************************************************************
Function name   : HD44780_PutStr
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Prints a string at the current character cursor position.

Special Note(s) : NONE

Parameters			: str			-	string (char array) to print

Return value		: NONE
*********************************************************************************************/
void HD44780_PutStr(char *str)
{
	__IO unsigned int i=0;

	do
	{
		HD44780_PutChar(str[i]);
		i++;
	}while(str[i]!='\0');
}



/*********************************************************************************************
Function name   : HD44780_ClrScr
Author 					: Grant Phillips
Date Modified   : 16/10/2013
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Clears the display

Special Note(s) : NONE

Parameters			: NONE
Return value		: NONE
*********************************************************************************************/
void HD44780_ClrScr(void)
{
	hd44780_wr_cmd(HD44780_CMD_CLEAR);
}


