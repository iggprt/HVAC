/*
 * adc.c
 *
 * Created: 9/19/2017 12:57:55 PM
 *  Author: uidn1932
 */ 
#include "avr/io.h"
#include "Global.h"
#include "avr/interrupt.h"
#include "HD44780.h"
sint16 result_ADC;
sint16 result_ADC_motor;

void adc_init()
{
	cli();
	
	ADMUX=0x40;

	ADCSRA|=1<<ADIE;
	ADCSRA|=6; //prscaler =64
	ADCSRA|=1<<ADEN;
	
	sei();
	
}
sint16 get_result_ADC(void)
{
	//result_ADC
	return result_ADC>>3;
}

sint16 get_result_ADC_motor(void)
{
	//result_ADC
	return result_ADC_motor>>3;
}

void adc_cyclic(uint8 channel)
{
	if(channel==0)
	{
		ADMUX &= 0xf8;
	}
	if(channel==1)
	{	
		ADMUX |= 1;
	}
	ADCSRA |= 1<<ADSC;
}


ISR(ADC_vect)
{
	if(ADMUX&1)
		result_ADC=ADC;
	else
	{
		
		result_ADC_motor=ADC;
	}
	
}