/*
 * pwm.c
 *
 * Created: 9/19/2017 2:47:30 PM
 *  Author: uidn1932
 */ 
#include "avr/io.h"
#include "avr/interrupt.h"
#include "z_events.h"
#include "HD44780.h"
#include "avr/delay.h"
#include "z_pwm.h"
#include "z_events.h"


char level=0;
void set_pwm_fan_lvl( unsigned char lvl)
{
	if(ON)
	{
		lvl%=4;
		if (lvl==0)
			level=FAN_LVL_0;
		else if(lvl==1)
			level=FAN_LVL_1;
		else if (lvl==2)
			level=FAN_LVL_2;
		else if (lvl==3)
			level=FAN_LVL_3;
	}
	else
	{
		level=FAN_LVL_0;
	}
	
	//if(OCR2>temp)
		//OCR2=count_gradient/50;
	//OCR2=lvl*85;
}
char pwm_pow_lvl;
void set_pwm_pow_lvl (unsigned char lvl)
{
	if(ON)
	{
			lvl%=4;
		if (lvl==0)
			pwm_pow_lvl = 0;
		else if(lvl==1)
			pwm_pow_lvl = 4;
		else if (lvl==2)
			pwm_pow_lvl = 300;
		else if (lvl==3)
			pwm_pow_lvl = 1000;
	}
	else
	{
		pwm_pow_lvl=0;
	}
	
}
void init_timer0()
{
	cli();
	 TCCR0|=2;
	TIMSK |= 1<<TOIE0; 
	TCNT0=0;
	sei();
}
void init_timer2()
{
	cli();
	
	OCR2=255-10;
	TCCR2=0x6b;
	TIMSK |= 1<<OCIE2;
	TIFR  |= 1<<OCF2; 
	ASSR &= 0xf0;
	
	sei();
}

double count_gradient=255*50;
ISR(TIMER2_COMP_vect)
{
	if(count_gradient>level*50)
		count_gradient--;
	if(count_gradient<level*50)
		count_gradient++;
	OCR2=count_gradient/50;
	

}
char count=0;
ISR(TIMER0_OVF_vect)
{
	char buf[4] = "0000";
	
	
	if ( count >= 1000 )
		count =0;
		
	if ( count < pwm_pow_lvl )
		PORTD &= ~(1<<6);
	else 
		PORTD |= 64;
		
	TCNT0=250;
	count++;
}