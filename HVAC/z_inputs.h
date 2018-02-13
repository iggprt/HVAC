/*
 * z_inputs.h
 *
 * Created: 9/18/2017 11:25:32 AM
 *  Author: uidn1932
 */ 

#ifndef Z_INPUTS_H_
#define Z_INPUTS_H_
typedef enum sw_state
{
	Bt_unpressed=0,
	Bt_pos_edge,
	Bt_pressed,
	Bt_neg_edge

}sw_state_e;

extern void debounce_D(volatile char *c);
extern void button_4();

#endif /*Z_INPUTS_H_*/