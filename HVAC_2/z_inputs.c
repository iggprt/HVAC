/*
 * inputs.c
 *
 * Created: 9/18/2017 11:25:06 AM
 *  Author: uidn1932
 */ 
#include "avr/io.h"
#include "avr/delay.h"
#include "z_inputs.h"
#include "Global.h"
#include "z_events.h"


uint16_t buffer[8]={0,0,0,0,0,0,0,0};
sw_state_e enum_state[8]={0,0,0,0,0,0,0,0};

void debounce_D(volatile uint8_t *port_button)
{
	uint8 port_button_poz=0;
	for (port_button_poz=0;port_button_poz<8;port_button_poz++)
	{
		buffer[port_button_poz]<<=1;
		if(((*port_button>>port_button_poz)&0x01)==0)
		{
			buffer[port_button_poz]|=0x01;
		}
		if(buffer[port_button_poz]==0x00ff && enum_state[port_button_poz]==Bt_unpressed)
		{
			enum_state[port_button_poz]=Bt_pos_edge;
			//set_event();
			PORTA=~PORTA;
			//setEvent(port_button_poz);
		}else
		if(buffer[port_button_poz]==0xffff && enum_state[port_button_poz]==Bt_pos_edge)
		{
			enum_state[port_button_poz]=Bt_pressed;
			//set_event();
		}else
		if((buffer[port_button_poz]&0x00ff)==0 && (buffer[port_button_poz]&0xff00)!=0 && enum_state[port_button_poz]==Bt_pressed)
		{
			enum_state[port_button_poz]=Bt_neg_edge;
			//set_event();
// 			if (port_button_poz==4)
// 			{
// 				setEvent(ev9_B_falling);
// 			}
		}else
		if(buffer[port_button_poz]==0x0000)
		{
			enum_state[port_button_poz]=Bt_unpressed;
		}
	}
	
}
