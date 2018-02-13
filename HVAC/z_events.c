
/*
 * z_events.c
 *
 * Created: 9/18/2017 12:26:01 PM
 *  Author: uidn1932
 */ 
#include "z_events.h"
#include "z_adc.h"
#include "z_pwm.h"
#include "avr/interrupt.h"
//char state;

signed char desired_temp;

void set_event(unsigned char ev)
{
	ev%=7;
	if(ev==0)
		states^=1;
		
	if(ON && !AUTO)
	{
	
		
		if(ev==1)
		{
			if((states&6)<6)
				states+=2;
			else
				states&=~(3<<1);
		}
		
		if(ev==2)
		{
			if((states&24)<24)
				states+=8;
			else
				states&=~(3<<3);
		}
		
		if(ev==3)
			states^=32;
			
		if(ev==4)
			states^=64;
			
		
	}
	else if(ON && AUTO)
	{
		if (ev==4)
			states ^= 64;
		if(ev==5)
		{
			if(desired_temp<36)
			desired_temp++;
		}
		
		if(ev==6)
		{
			if(desired_temp>(-13))
			desired_temp--;
		}	
			
	}
	
}
signed char temp()
{
	
	adc_cyclic(0);
	return (signed char)(get_result_ADC()/2.5 - 14);
	
}

signed char motor()
{
	adc_cyclic(1);
	return get_result_ADC_motor();
}

void auto_climate_control()
{
	char u8temp=temp();
	if(ON && AUTO)
	{
		if(desired_temp>u8temp)
		{
			SET_HEAT_ON;
			if(desired_temp-u8temp>7)
			{
				SET_FAN_3;
				SET_POW_3;
			}
			else
			{
				SET_FAN_2;
				SET_POW_1;
			}
			
		}
		if(desired_temp<u8temp)
		{
			
			SET_HEAT_OFF;
			if(u8temp-desired_temp>7)
			{
				SET_FAN_3;
				SET_POW_2;
			}
			else
			{
				SET_FAN_1;
				SET_POW_1;
			}
		}
		if(desired_temp==u8temp)
		{
			SET_FAN_0;
			SET_POW_0;
		}
		set_pwm_fan_lvl(FAN);
		set_pwm_pow_lvl(POW);
	}
}
