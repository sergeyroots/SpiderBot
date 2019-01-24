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


//static const sg90_hw_t sg90HW[] = {
	//SERVO_D2,  SERVO_D3,  SERVO_D4,
	//SERVO_D5,  SERVO_D6,  SERVO_D7,
	//SERVO_D8,  SERVO_D9,  SERVO_D10,
	//SERVO_D11, SERVO_D12, SERVO_D13,
	//SERVO_A0,  SERVO_A1,  SERVO_A2,
	//SERVO_A3,  SERVO_A4,  SERVO_A5,
//};
//
//static const sg90_init_t sg90InitData = {
	//6, // footCount
	//3, // bendCount
	//sg90HW
//};

static const sg90_hw_t sg90HW[] = {
	SERVO_D12
};

static const sg90_init_t sg90InitData = {
	1, // footCount
	1, // bendCount
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
static const uint8_t cmd_data_forward[] PROGMEM = {
	CMD_A(10), CMD_A(11), CMD_A(12), CMD_A(13), CMD_A(14), CMD_A(15), CMD_A(16), CMD_A(17), CMD_A(18), CMD_A(19), 
	CMD_A(20), CMD_A(21), CMD_A(22), CMD_A(23), CMD_A(24), CMD_A(25), CMD_A(26), CMD_A(27), CMD_A(28), CMD_A(29), 
	CMD_A(30), CMD_A(31), CMD_A(32), CMD_A(33), CMD_A(34), CMD_A(35), CMD_A(36), CMD_A(37), CMD_A(38), CMD_A(39), 
	CMD_A(40), CMD_A(41), CMD_A(42), CMD_A(43), CMD_A(44), CMD_A(45), CMD_A(46), CMD_A(47), CMD_A(48), CMD_A(49), 
	CMD_A(50), CMD_A(51), CMD_A(52), CMD_A(53), CMD_A(54), CMD_A(55), CMD_A(56), CMD_A(57), CMD_A(58), CMD_A(59), 
	CMD_A(60), CMD_A(61), CMD_A(62), CMD_A(63), CMD_A(64), CMD_A(65), CMD_A(66), CMD_A(67), CMD_A(68), CMD_A(69), 
	CMD_A(70), CMD_A(71), CMD_A(72), CMD_A(73), CMD_A(74), CMD_A(75), CMD_A(76), CMD_A(77), CMD_A(78), CMD_A(79), 
	CMD_A(80), CMD_A(81), CMD_A(82), CMD_A(83), CMD_A(84), CMD_A(85), CMD_A(86), CMD_A(87), CMD_A(88), CMD_A(89), 
	CMD_A(90), CMD_A(91), CMD_A(92), CMD_A(93), CMD_A(94), CMD_A(95), CMD_A(96), CMD_A(97), CMD_A(98), CMD_A(99), 
	CMD_A(100), CMD_A(101), CMD_A(102), CMD_A(103), CMD_A(104), CMD_A(105), CMD_A(106), CMD_A(107), CMD_A(108), CMD_A(109),
	CMD_A(110), CMD_A(111), CMD_A(112), CMD_A(113), CMD_A(114), CMD_A(115), CMD_A(116), CMD_A(117), CMD_A(118), CMD_A(119), 
	CMD_A(120), CMD_A(121), CMD_A(122), CMD_A(123), CMD_A(124), CMD_A(125), CMD_A(126), CMD_A(127), CMD_A(128), CMD_A(129), 
	CMD_A(130), CMD_A(131), CMD_A(132), CMD_A(133), CMD_A(134), CMD_A(135), CMD_A(136), CMD_A(137), CMD_A(138), CMD_A(139), 
	CMD_A(140), CMD_A(141), CMD_A(142), CMD_A(143), CMD_A(144), CMD_A(145), CMD_A(146), CMD_A(147), CMD_A(148), CMD_A(149), 
	CMD_A(150), CMD_A(151), CMD_A(152), CMD_A(153), CMD_A(154), CMD_A(155), CMD_A(156), CMD_A(157), CMD_A(158), CMD_A(159), 
	CMD_A(160), CMD_A(161), CMD_A(162), CMD_A(163), CMD_A(164), CMD_A(165), CMD_A(166), CMD_A(167), CMD_A(168), CMD_A(169), 
	CMD_A(170),
	
	CMD_A(170), CMD_A(169), CMD_A(168), CMD_A(167), CMD_A(166), CMD_A(165), CMD_A(164), CMD_A(163), CMD_A(162),	CMD_A(161), 
	CMD_A(160), CMD_A(159), CMD_A(158), CMD_A(157), CMD_A(156), CMD_A(155), CMD_A(154), CMD_A(153),	CMD_A(152), CMD_A(151), 
	CMD_A(150), CMD_A(149), CMD_A(148), CMD_A(147), CMD_A(146), CMD_A(145), CMD_A(144),	CMD_A(143), CMD_A(142), CMD_A(141), 
	CMD_A(140), CMD_A(139), CMD_A(138), CMD_A(137), CMD_A(136), CMD_A(135),	CMD_A(134), CMD_A(133), CMD_A(132), CMD_A(131), 
	CMD_A(130), CMD_A(129), CMD_A(128), CMD_A(127), CMD_A(126),	CMD_A(125), CMD_A(124), CMD_A(123), CMD_A(122), CMD_A(121), 
	CMD_A(120), CMD_A(119), CMD_A(118), CMD_A(117),	CMD_A(116), CMD_A(115), CMD_A(114), CMD_A(113), CMD_A(112), CMD_A(111), 
	CMD_A(110), CMD_A(109), CMD_A(108),	CMD_A(107), CMD_A(106), CMD_A(105), CMD_A(104), CMD_A(103), CMD_A(102), CMD_A(101), 
	CMD_A(100), CMD_A(99), CMD_A(98), CMD_A(97), CMD_A(96), CMD_A(95), CMD_A(94), CMD_A(93), CMD_A(92), CMD_A(91), 
	CMD_A(90), CMD_A(89), CMD_A(88), CMD_A(87), CMD_A(86), CMD_A(85), CMD_A(84), CMD_A(83), CMD_A(82), CMD_A(81), 
	CMD_A(80), CMD_A(79), CMD_A(78), CMD_A(77), CMD_A(76), CMD_A(75), CMD_A(74), CMD_A(73), CMD_A(72), CMD_A(71), 
	CMD_A(70), CMD_A(69), CMD_A(68), CMD_A(67), CMD_A(66), CMD_A(65), CMD_A(64), CMD_A(63), CMD_A(62), CMD_A(61), 
	CMD_A(60), CMD_A(59), CMD_A(58), CMD_A(57), CMD_A(56), CMD_A(55), CMD_A(54), CMD_A(53), CMD_A(52), CMD_A(51), 
	CMD_A(50), CMD_A(49), CMD_A(48), CMD_A(47), CMD_A(46), CMD_A(45), CMD_A(44), CMD_A(43), CMD_A(42), CMD_A(41), 
	CMD_A(40), CMD_A(39), CMD_A(38), CMD_A(37), CMD_A(36), CMD_A(35), CMD_A(34), CMD_A(33), CMD_A(32), CMD_A(31), 
	CMD_A(30), CMD_A(29), CMD_A(28), CMD_A(27), CMD_A(26), CMD_A(25), CMD_A(24), CMD_A(23), CMD_A(22), CMD_A(21), 
	CMD_A(20), CMD_A(19), CMD_A(18), CMD_A(17), CMD_A(16), CMD_A(15), CMD_A(14), CMD_A(13), CMD_A(12), CMD_A(11), 
	CMD_A(10)
};

//static const uint8_t cmd_data_forward[] PROGMEM = {
	//CMD_A(45), CMD_A(90), CMD_A(135),
	//CMD_A(90), CMD_A(45)
//};


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
	{H_COMMAND_FORWARD, cmd_data_forward, 255, CMD_DELAY_50_MS}
};

#endif /* COMMANDS_H_ */