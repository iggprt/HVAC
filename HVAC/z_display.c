
/*
 * display.c
 *
 * Created: 9/20/17 12:31:09 AM
 *  Author: iggprt
 */ 
#include "avr/io.h"
#include "z_pwm.h"
#include "z_adc.h"
#include "z_events.h"

void display_phase()
{
	
	char buf[4]="0000";
	
	LCD_Clear();
	
	if ( ON )
	{
		LCD_GoTo(0,0);
		LCD_WriteText("P:");
		
		
		itoa(POW,buf,10);
		LCD_GoTo(2,0);
		LCD_WriteText(buf);
		
		LCD_GoTo(4,0);
		LCD_WriteText("F:");
		
		itoa(FAN,buf,10);
		LCD_GoTo(6,0);
		LCD_WriteText(buf);
		
		
		LCD_GoTo(8,0);
		if(HEAT)
		LCD_WriteText("H");
		else 
		LCD_WriteText("C");
		
		
		
		LCD_GoTo(11,0);
		LCD_WriteText("W:");
		
		itoa(desired_temp, buf,10);
		LCD_GoTo(13,0);
		LCD_WriteText(buf);
		
		LCD_GoTo(0,1);
		LCD_WriteText("T:");
		
		itoa(temp(), buf,10);
		LCD_GoTo(2,1);
		LCD_WriteText(buf);
		
		LCD_GoTo(9,0);
		if(AUTO)
		LCD_WriteText("A");
		else 
		LCD_WriteText("M");
		
		
		
		
	}
	else 
	{
		LCD_GoTo(0,0);
		LCD_WriteText("OFF");
		
	}
	
}