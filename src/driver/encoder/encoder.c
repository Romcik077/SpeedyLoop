/*
 * encoder.c
 *
 *  Created on: Jan 22, 2014
 *      Author: proman
 */

#include "encoder.h"

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

//	// Initialize timer0 for speed control
//	timer0Init(TIMER_CLK_DIV1024);
//	timer0CTCInit();
//	timer0CTCSetPeriod(20);
//	timer0Attach(TIMER0_OUTCOMPAREA_INT, encoderProcesingData);

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

// Every 10ms this function will be call for calculate speed motor, using timer0
void encoderProcesingData(void)
{
	timerCount++;
	if(timerCount > 9)
	{
		// 120 - 5 tooth, 200 - 3 tooth
		if(encoder1Direction == ENCODER_FORWARD)
		{
			encoder1Speed = (encoder1Count*200.0);///5.0)*(600.0);///3.0)*(600.0);
		} else {
			encoder1Speed = -(encoder1Count*200.0);
		}

		if(encoder1Direction == ENCODER_FORWARD)
		{
			encoder2Speed = (encoder2Count*200.0);///5.0)*(600.0);///3.0)*(600.0);
		} else {
			encoder2Speed = -(encoder2Count*200.0);
		}

		encoder1Count = 0;
		encoder2Count = 0;

		timerCount = 0;
	}
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
			encoder1Direction = ENCODER_FORWARD;
		} else {
			encoder1Count++;
			encoder1Direction = ENCODER_BACKWARD;
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
