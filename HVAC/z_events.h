
/*
 * z_events.h
 *
 * Created: 9/18/2017 12:27:14 PM
 *  Author: uidn1932
 */ 
#ifndef Z_EVENTS_H_
#define Z_EVENTS_H_


// char state;
unsigned char events;
unsigned char states;

//#define FAN_ON 9

typedef enum 
{
	on_off = 0,
	fan_lvl,
	heat_cold,
	power
	
}events_e;  

#define TEMP_MAX 50
#define TEMP_MIN -20

#define ON        states&1
#define POW  (states>>1)&3
#define FAN  (states>>3)&3
#define HEAT (states>>5)&1
#define AUTO (states>>6)&1

#define SET_POW_0 (states &= 0b11111001)
#define SET_POW_1 (states = (states & 0b11111001) + 2)
#define SET_POW_2 (states = (states & 0b11111001) + 4)
#define SET_POW_3 (states = (states & 0b11111001) + 6)

#define SET_FAN_0 (states &= 0b11100111)
#define SET_FAN_1 (states = (states & 0b11100111) + 8)
#define SET_FAN_2 (states = (states & 0b11100111) + 16)
#define SET_FAN_3 (states = (states & 0b11100111) + 24)

#define SET_HEAT_ON states|= 1<<5
#define SET_HEAT_OFF states &= ~(1<<5)




extern void auto_climate_control();
extern signed char motor();

extern signed char desired_temp;

#endif /*Z_EVENTS_H_*/