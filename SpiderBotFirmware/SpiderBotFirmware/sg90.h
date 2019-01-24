#ifndef _SG90_H_
#define _SG90_H_

#include "spiderbotfirmware.h"

typedef struct {
	volatile uint8_t *port;
	uint8_t pinMask;	
} sg90_hw_t;

typedef struct {
	uint8_t footCount;
	uint8_t bendCount;
	const sg90_hw_t *hw;
} sg90_init_t;

extern uint8_t sg90_servoCount;

void sg90_init(const sg90_init_t *initData);
void sg90_setAngle(uint8_t servoNum, int8_t angle);
void sg90_setAllServoAngles(uint8_t *angles);

#endif /* _SG90_H_ */