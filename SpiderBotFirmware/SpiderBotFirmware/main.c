/*
 * main.c
 *
 * Created: 27.12.2018 10:11:39
 * Author : ortman sergey
 */ 

#include "main.h"
#include "commands.h"

int main(void) {
    portsInit();
	cmd_init();	
	sei();
	//if (cmd_getCurrentCommand() == H_COMMAND_NOP) {
		//cmd_run(H_COMMAND_FORWARD);
	//}
	//sg90_setAngle(0, 45);
    while (1) {
		cmd_run(H_COMMAND_FORWARD);
	    //PORTB |= LED;
		//_delay_ms(10000);		
		//PORTB &= ~LED;
		//PORTB ^= LED;		
    }
}

