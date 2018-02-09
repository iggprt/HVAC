/*
 * lcd_custom_sym.c
 *
 * Created: 15-Sep-17 1:23:04 PM
 *  Author: uidp0102
 */ 

#include "lcd_custom_sym.h"

#define USE_PROGRAM_MEMORY

#if defined(USE_PROGRAM_MEMORY)
const char lcd_custom_sym[8][8] PROGMEM  =
#else
const char lcd_custom_sym[8][8] =
#endif
 {
	/* '\0' */
	{
		 0b00000
		,0b10011
		,0b11010
		,0b00100
		,0b01011
		,0b11001
		,0b00000
		,0b00000

	},	
	/* '\1' */
	{
		 0b00000
		,0b00110
		,0b10100
		,0b11111
		,0b00101
		,0b01100
		,0b00000
		,0b00000
	},
	/* '\2' */
	{
		 0b00000
		,0b10011
		,0b11110
		,0b00100
		,0b01111
		,0b11001
		,0b00000
		,0b00000
	},
	/* '\3' */
	{
		 0b00000
		,0b00110
		,0b10100
		,0b11111
		,0b00101
		,0b01100
		,0b00000
		,0b00000
	},
	/* '\4' */
	{
		 0b01000
		,0b10101
		,0b00010
		,0b00000
		,0b01000
		,0b10101
		,0b00010
		,0b00000
	},
	/* '\5' */
	{
		 0b00100
		,0b01010
		,0b10001
		,0b00000
		,0b00100
		,0b01010
		,0b10001
		,0b00000
	},
	/* '\6' */
	{
		 0b00010
		,0b10101
		,0b01000
		,0b00000
		,0b00010
		,0b10101
		,0b01000
		,0b00000
	},
	/* '\7' */
	{
		 0b10001
		,0b01010
		,0b00100
		,0b00000
		,0b10001
		,0b01010
		,0b00100
		,0b00000
	}
};


void LCD_FillGRAM(void)
{
	char buffer[8];
	int i,j;
	for (i=0;i<8;i++)
	{
		for (j=0;j<8;j++)
		{
			#if defined(USE_PROGRAM_MEMORY)
			buffer[j] = pgm_read_byte_near(&(lcd_custom_sym[i][j]));
			#else
			buffer[j] = lcd_custom_sym[i][j];
			#endif
		}
		LCD_AddCustomSymbol(i,buffer);
	}
	
}
