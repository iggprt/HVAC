/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 *
 * Main. c also creates a task called "Check".  This only executes every three
 * seconds but has the highest priority so is guaranteed to get processor time.
 * Its main function is to check that all the other tasks are still operational.
 * Each task that does not flash an LED maintains a unique count that is
 * incremented each time the task successfully completes its function.  Should
 * any error occur within such a task the count is permanently halted.  The
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have
 * changed all the tasks are still executing error free, and the check task
 * toggles an LED.  Should any task contain an error at any time the LED toggle
 * will stop.
 *
 * The LED flash and communications test tasks do not maintain a count.
 */

/*
Changes from V1.2.0

	+ Changed the baud rate for the serial test from 19200 to 57600.

Changes from V1.2.3

	+ The integer and comtest tasks are now used when the cooperative scheduler
	  is being used.  Previously they were only used with the preemptive
	  scheduler.

Changes from V1.2.5

	+ Set the baud rate to 38400.  This has a smaller error percentage with an
	  8MHz clock (according to the manual).

Changes from V2.0.0

	+ Delay periods are now specified using variables and constants of
	  TickType_t rather than unsigned long.

Changes from V2.6.1

	+ The IAR and WinAVR AVR ports are now maintained separately.

Changes from V4.0.5

	+ Modified to demonstrate the use of co-routines.

*/

#include <stdlib.h>
#include <string.h>

	
#include "lcd_custom_sym.h"

#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h>
#endif

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

#include "z_events.h"
#include "z_inputs.h"
#include "z_adc.h"
#include "z_pwm.h"
#include "z_display.h"

/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainCHECK_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )


/* The period between executions of the check task. */
#define mainCHECK_PERIOD				( ( TickType_t ) 3000 / portTICK_PERIOD_MS  )

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )


/*
 * Called on boot to increment a count stored in the EEPROM.  This is used to
 * ensure the CPU does not reset unexpectedly.
 */
static void prvIncrementResetCount( void );

/*
 * The idle hook is used to scheduler co-routines.
 */
void vApplicationIdleHook( void );

static void vLCD_TASK(void *pvParameters);
static void tBlinkLED(void *pvParameters);
static void display  (void *pvParameters);
static void debug_   (void *pvParameters);
static void temperature(void *pvParameters);
/*-----------------------------------------------------------*/

int main( void )
{
	prvIncrementResetCount();
	
	
	
	//events|=128;
	
	DDRB=0;
	PORTB=0xff;
	
	DDRD = 0xff;
	PORTD=0xff;
	
	DDRA=0;
	PORTA=0xff;
	
	desired_temp=20;
	
	adc_init();
	 
	init_timer2();
	init_timer0();
	 
	LCD_Initalize();
	LCD_FillGRAM();

	/* Create the tasks defined within this file. */
	//xTaskCreate( vLCD_TASK, "LCD", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	//xTaskCreate( tBlinkLED, "Blink", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
	xTaskCreate( display, "debug", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
	xTaskCreate( debug_, "debug_", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
	xTaskCreate( temperature, "temperaturee", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
	
	/* In this port, to use preemptive scheduler define configUSE_PREEMPTION
	as 1 in portmacro.h.  To use the cooperative scheduler define
	configUSE_PREEMPTION as 0. */
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

static void tBlinkLED(void *pvParameters)
{
	(void)pvParameters;
	for(;;)
	{
		DDRB |= 0xff;
		PORTB = (PORTB > 0 ? PORTB << 1 : 1);
		asm("nop");
		vTaskDelay(20);
	}
}

static void vLCD_TASK( void *pvParameters )
{
	(void)pvParameters;
	char buf[5] = "0000";
	int i = 0,j=0,k=0;
	int flag = 0;
	for( ;; )
	{
		i++;
		itoa(i,buf,10);
		LCD_GoTo(0,0);
		LCD_WriteData(15);
		LCD_GoTo(0,1);
		LCD_WriteText(buf);
		if(flag%2)
		{
			LCD_GoTo(7,1);
			LCD_ShowCustomChar((j%2));
			j++;
		}
		else
		{
			LCD_GoTo(8,1);
			LCD_ShowCustomChar((k%4)+4);
			k++;
		}
		flag++;
		vTaskDelay(50*2);
	}
}
//______________________________________________________________________________________
static void display(void *pvParameters)
{
	(void)pvParameters;
	char buf[4]="0";
	for(;;)
	{
		
		
		
		display_phase();
		
	
		set_pwm_pow_lvl(POW);
		set_pwm_fan_lvl(FAN);
		
		//PORTD&=~((PORTD<<6)&PIND7);
				
				/*--------------------------------------------motor--feedback----------------------*/
				itoa(motor(),buf,10);
				LCD_GoTo(6,1);
				LCD_WriteText("m_fb:");
				LCD_GoTo(11,1);
				LCD_WriteText(buf);
				/*---------------------------------------------------------------------------------*/
		vTaskDelay(70);
		//vTaskDelayuntil();
	}
}
static void debug_(void *pvParameters)
{
	(void)pvParameters;
	for(;;)
	{
		debounce_D(&PINB);
		vTaskDelay(3);
	}
	
}

static void temperature(void *pvParameters)
{
	(void)pvParameters;
	char buf[4]="0000";
	for(;;)
	{
		auto_climate_control();
		vTaskDelay(55);

	}
	
}

/*-----------------------------------------------------------*/

static void prvIncrementResetCount( void )
{
unsigned char ucCount;

	eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
	ucCount++;
	eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{

}

