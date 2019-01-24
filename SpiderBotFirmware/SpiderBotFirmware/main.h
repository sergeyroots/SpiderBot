/*
 * main.h
 *
 * Created: 28.12.2018 9:18:11
 *  Author: ortman
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include "spiderbotfirmware.h"

inline void portsInit(void) {
	PORTB = 0;	
	PORTC = 0;
	PORTD = 0;
	DDRB = LED;
	DDRC = 0;
	DDRD = 0;
}

#endif /* MAIN_H_ */