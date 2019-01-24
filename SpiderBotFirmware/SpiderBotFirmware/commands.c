/*
 * commands.c
 *
 * Created: 28.12.2018 19:31:31
 *  Author: ortman
 */ 
#include "commands.h"
#include <string.h>
#include <stdlib.h>

#define TIMER0_STOP					0
#define TIMER0_PRESCALER_1			(1<<CS00)
#define TIMER0_PRESCALER_8			(1<<CS01)
#define TIMER0_PRESCALER_64			((1<<CS00)|(1<<CS01))
#define TIMER0_PRESCALER_256		(1<<CS02)
#define TIMER0_PRESCALER_1024		((1<<CS00)|(1<<CS02))
#define TIMER0_EXTERNAL_CLK_FALLING	((1<<CS01)|(1<<CS02))
#define TIMER0_EXTERNAL_CLK_RISIING	((1<<CS00)|(1<<CS01)|(1<<CS02))

#define TIMER0_MODE_NORMAL	(0)
#define TIMER0_MODE_CTC		(1<<WGM01)
#define TIMER0_ISR_EN		(1<<OCIE0A)

h_command_data_t cmd_currentCommand = {H_COMMAND_NOP, 0, 0, 0};
uint8_t cmd_iteration = 0xFF;
uint8_t cmd_iterationDelay = 0;
uint8_t allCommandsCount = 0;
uint8_t *cmd_iterationAngles;

void cmd_init(void) {
	allCommandsCount = sizeof(commands_all) / sizeof(h_command_data_t);
	sg90_init(&sg90InitData);
	cmd_iterationAngles = malloc(sg90_servoCount);
	TCCR0B = TIMER0_PRESCALER_1024;
	TCCR0A = TIMER0_MODE_NORMAL;
	TIMSK0 = TIMER0_ISR_EN;	
}

void cmd_run(h_command_t cmd) {
	if (cmd_currentCommand.command == H_COMMAND_NOP) {
		uint8_t i = 0;	
		for (; i < allCommandsCount; ++i) {
			if (commands_all[i].command == cmd) {
				PORTB ^= LED;
				cmd_currentCommand.command = cmd;
				memcpy(&cmd_currentCommand, &commands_all[i], sizeof(h_command_data_t));				
				uint8_t *dataPtr = (uint8_t*)cmd_currentCommand.data;
				uint8_t *anglesPtd = cmd_iterationAngles;
				for (uint8_t i=0; i < sg90_servoCount; ++i) {
					*anglesPtd = pgm_read_byte(dataPtr++);
					anglesPtd++;
				}
				cmd_iterationDelay = 0;
				cmd_iteration = 0;
				break;
			}
		}
	}
}

h_command_t cmd_getCurrentCommand(void) {
	return cmd_currentCommand.command;
}

void cmd_hardStop(void) {
	
}

void cmd_softStop(void) {
	
}

ISR (TIMER0_COMPA_vect) {
	if (cmd_iteration != 0xFF) {
		if (cmd_iterationDelay < cmd_currentCommand.delay_tic) {
			cmd_iterationDelay++;
		} else {
			sg90_setAllServoAngles(cmd_iterationAngles);
			cmd_iteration++;
			if (cmd_iteration >= cmd_currentCommand.iterationCount) {
				cmd_iteration = 0xFF;
				cmd_currentCommand.command = H_COMMAND_NOP;
			} else {
				cmd_iterationDelay = 0;
				uint8_t *dataPtr = (uint8_t*)&cmd_currentCommand.data[cmd_iteration * sg90_servoCount];
				uint8_t *anglesPtd = cmd_iterationAngles;
				for (uint8_t i=0; i < sg90_servoCount; ++i) {
					*anglesPtd = pgm_read_byte(dataPtr++);
					anglesPtd++;
				}
			}
		}
	}
}