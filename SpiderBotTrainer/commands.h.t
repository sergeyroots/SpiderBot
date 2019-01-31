/*
 * commands.h
 *
 *  Author: ortman
 */ 
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <avr/pgmspace.h>

#include "spiderbotfirmware.h"
#include "sg90.h"

/************************************************************************/
/* SETTINGS                                                             */
/************************************************************************/
#define SERVO_D2 {&PORTD, (1<<PD2)}
#define SERVO_D3 {&PORTD, (1<<PD3)}
#define SERVO_D4 {&PORTD, (1<<PD4)}
#define SERVO_D5 {&PORTD, (1<<PD5)}
#define SERVO_D6 {&PORTD, (1<<PD6)}
#define SERVO_D7 {&PORTD, (1<<PD7)}

#define SERVO_D8  {&PORTB, (1<<PB0)}
#define SERVO_D9  {&PORTB, (1<<PB1)}
#define SERVO_D10 {&PORTB, (1<<PB2)}
#define SERVO_D11 {&PORTB, (1<<PB3)}
#define SERVO_D12 {&PORTB, (1<<PB4)}
#define SERVO_D13 {&PORTB, (1<<PB5)}

#define SERVO_A0 {&PORTC, (1<<PC0)}
#define SERVO_A1 {&PORTC, (1<<PC1)}
#define SERVO_A2 {&PORTC, (1<<PC2)}
#define SERVO_A3 {&PORTC, (1<<PC3)}
#define SERVO_A4 {&PORTC, (1<<PC4)}
#define SERVO_A5 {&PORTC, (1<<PC5)}

#define SERVO_1		SERVO_D2
#define SERVO_2		SERVO_D3
#define SERVO_3		SERVO_D4
#define SERVO_4		SERVO_D5
#define SERVO_5		SERVO_D6
#define SERVO_6		SERVO_D7
#define SERVO_7		SERVO_D8
#define SERVO_8		SERVO_D9
#define SERVO_9		SERVO_D10
#define SERVO_10	SERVO_D11
#define SERVO_11	SERVO_D12
#define SERVO_12	SERVO_D13
#define SERVO_13	SERVO_A0
#define SERVO_14	SERVO_A1
#define SERVO_15	SERVO_A2
#define SERVO_16	SERVO_A3
#define SERVO_17	SERVO_A4
#define SERVO_18	SERVO_A5

static const sg90_hw_t sg90HW[] = {
%%SERVO_ARRAY%%
};

static const sg90_init_t sg90InitData = {
	%%FOOT_COUNT%%, // footCount
	%%SEGMENT_COUNT%%, // bendCount
	sg90HW
};

typedef enum {
	H_COMMAND_NOP = 0x00,
	
	H_COMMAND_FORWARD = 0x10,
	H_COMMAND_BACKWARD = 0x11,
} h_command_t;

typedef struct {
	h_command_t command;
	const uint8_t *data;
	uint16_t iterationCount;
	uint8_t delay_tic;  // 0 tic = 16.384 ms ((delay_tic+1)*16.384ms) (1000ms / (16000000/256/1024))
} h_command_data_t;

void cmd_init(void);
void cmd_run(h_command_t cmd);
h_command_t cmd_getCurrentCommand(void);
void cmd_hardStop(void);
void cmd_softStop(void);

#define CMD_A(a) (a*33/45+24)

/************************************************************************/
/* COMMANDS DATA                                                        */
/************************************************************************/
%%CMD_DATA_ARRAYS%%


/************************************************************************/
/* COMMANDS                                                             */
/************************************************************************/
#define CMD_DELAY_50_MS		3
#define CMD_DELAY_100_MS	6
#define CMD_DELAY_250_MS	15
#define CMD_DELAY_500_MS	30
#define CMD_DELAY_1_S		60
#define CMD_DELAY_2_S		120

static const h_command_data_t commands_all[] = {
%%CMD_ARRAYS%%
};

#endif /* COMMANDS_H_ */