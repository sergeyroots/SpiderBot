#include "sg90.h"
#include <string.h>
#include <stdlib.h>

#define TIMER1_STOP					0
#define TIMER1_PRESCALER_1			(1<<CS10)
#define TIMER1_PRESCALER_8			(1<<CS11)
#define TIMER1_PRESCALER_64			((1<<CS10)|(1<<CS11))
#define TIMER1_PRESCALER_256			(1<<CS12)
#define TIMER1_PRESCALER_1024		((1<<CS10)|(1<<CS12))
#define TIMER1_EXTERNAL_CLK_FALLING	((1<<CS11)|(1<<CS12))
#define TIMER1_EXTERNAL_CLK_RISIING	((1<<CS10)|(1<<CS11)|(1<<CS12))

#define TIMER1_MODE_CTC		(1<<WGM12)
#define TIMER1_ISR_EN		(1<<OCIE1A)

uint16_t sg90_counter = 0;
const sg90_init_t *sg90_initData = 0;
uint8_t *sg90_servoAngles;
uint8_t sg90_servoCount = 0;

void sg90_init(const sg90_init_t *initData) {
	sg90_servoCount = initData->footCount * initData->bendCount;
	volatile uint8_t *p;
	for (uint8_t i=0; i<sg90_servoCount; ++i) {
		p = initData->hw[i].port;
		*p = *p & ~initData->hw[i].pinMask;
		p = p - 1; // DirReg
		*p = *p | initData->hw[i].pinMask;
	}
	sg90_servoAngles = malloc(sg90_servoCount);
	sg90_initData = initData;
	TCCR1B = TIMER1_MODE_CTC | TIMER1_PRESCALER_256;
	OCR1A = 0;
	TCNT1 = 0;
	TIMSK1 |= TIMER1_ISR_EN;
}

void sg90_setAngle(uint8_t servoNum, int8_t angle) {
	if (servoNum < sg90_servoCount) {
		sg90_servoAngles[servoNum] = angle;
	}
}

void sg90_setAllServoAngles(uint8_t *angles) {
	memcpy(sg90_servoAngles, angles, sg90_servoCount);
}

ISR (TIMER1_COMPA_vect) {
	sg90_counter++;
	if (sg90_counter > 1250) {
		sg90_counter = 0;
		for (uint8_t i=0; i< sg90_servoCount; i++) {
			*sg90_initData->hw[i].port = *sg90_initData->hw[i].port | sg90_initData->hw[i].pinMask;			
		}
	}	
	for (uint8_t i=0; i< sg90_servoCount; i++) {
		if (sg90_counter == (uint16_t)sg90_servoAngles[i]) {
			*sg90_initData->hw[i].port = *sg90_initData->hw[i].port & ~sg90_initData->hw[i].pinMask;
		}
	}
}