/*
 * encoder.c
 *
 *  Created on: Jan 22, 2014
 *      Author: proman
 */

#include "encoder.h"

#define MEASURE_PERIOD_MS	40
#define NUMBER_OF_TOOTHS	3

// (encoderCount/numberTooths)*(60000/measurePeriodms)
const double encoderMeasureCoeficient = (60000.0/MEASURE_PERIOD_MS)/NUMBER_OF_TOOTHS;

// Declaration of variables
volatile double encoder1Speed;
volatile double encoder2Speed;
volatile uint8_t encoder1Direction;
volatile uint8_t encoder2Direction;
volatile double encoder1Count;
volatile double encoder2Count;
volatile uint8_t out1APreState;
volatile uint8_t out2APreState;
volatile uint8_t timerCount;

void encoderInit(void)
{
	// Init OUT pins as intput
	cbi(OUT1A_DDR, OUT1A_PIN);
	cbi(OUT1B_DDR, OUT1B_PIN);
	cbi(OUT2A_DDR, OUT2A_PIN);
	cbi(OUT2B_DDR, OUT2B_PIN);

	// Init interrupts for input encoders
	sbi(PCMSK2, OUT1A_PCINT);
	sbi(PCMSK2, OUT2A_PCINT);
	sbi(PCICR, PCIE2);

	// Enable global interrupts
	sei();

	encoder1Count = 0;
	encoder2Count = 0;
	encoder1Speed = 0;
	encoder2Speed = 0;
	encoder1Direction = ENCODER_STOP;
	encoder2Direction = ENCODER_STOP;
	out1APreState = 0;
	out2APreState = 0;
	timerCount = 0;
}

// Return speed in RPM
double encoderGetSpeed(uint8_t encoder)
{
	switch (encoder) {
		case ENCODER1:
			return encoder1Speed;
			break;
		case ENCODER2:
			return encoder2Speed;
			break;
		default:
			break;
	}
	return -1;
}

// Return direction of motor
uint8_t encoderGetDirection(uint8_t encoder)
{
	switch (encoder) {
		case ENCODER1:
			return encoder1Direction;
			break;
		case ENCODER2:
			return encoder2Direction;
		default:
			break;
	}
	return -1;
}

// Every 50ms this function need to be call for calculate speed motor
void encoderProcesingData(void)
{
		if(encoder1Direction == ENCODER_FORWARD)
		{
			encoder1Speed = (encoder1Count*encoderMeasureCoeficient);
		} else {
			encoder1Speed = 0 - (encoder1Count*encoderMeasureCoeficient);
		}

		if(encoder2Direction == ENCODER_FORWARD)
		{
			encoder2Speed = (encoder2Count*encoderMeasureCoeficient);
		} else {
			encoder2Speed = 0 - (encoder2Count*encoderMeasureCoeficient);
		}

		encoder1Count = 0;
		encoder2Count = 0;
}

// This interrupt using for enumerate the changes state of encoders
ISR(PCINT2_vect)
{
	uint8_t out1AState = PIND & (1 << OUT1A_PIN);
	uint8_t out1BState = PIND & (1 << OUT1B_PIN);
	uint8_t out2AState = PIND & (1 << OUT2A_PIN);
	uint8_t out2BState = PIND & (1 << OUT2B_PIN);

	if((out1AState) && (!out1APreState))
	{
		if(out1BState)
		{
			encoder1Count++;
			encoder1Direction = ENCODER_BACKWARD;
		} else {
			encoder1Count++;
			encoder1Direction = ENCODER_FORWARD;
		}
	}

	if((out2AState) && (!out2APreState))
	{
		if(out2BState)
		{
			encoder2Count++;
			encoder2Direction = ENCODER_FORWARD;
		} else {
			encoder2Count++;
			encoder2Direction = ENCODER_BACKWARD;
		}
	}

	out1APreState = out1AState;
	out2APreState = out2AState;
}
