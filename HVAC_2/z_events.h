
/*
 * z_events.h
 *
 * Created: 9/18/2017 12:27:14 PM
 *  Author: uidn1932
 */ 
#ifndef Z_EVENTS_H_
#define Z_EVENTS_H_


// char state;
char events;

#define FAN_ON 9

typedef enum 
{
	on_off = 0,
	fan_lvl,
	heat_cold,
	power
	
}events_e;  

#endif Z_EVENTS_H_