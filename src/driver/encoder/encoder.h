/*
 * encoder.h
 *
 *  Created on: Jan 22, 2014
 *      Author: proman
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <avr/io.h>
#include <global.h>
#include <avr/interrupt.h>

// Configurations of pins
#define ENCODER1	0
#define OUT1A_PORT	PORTD
#define OUT1B_PORT	PORTD
#define OUT1A_DDR	DDRD
#define OUT1B_DDR	DDRD
#define OUT1A_PIN	PD3
#define OUT1B_PIN	PD4
#define OUT1A_PCINT	PCINT19
#define OUT1B_PCINT	PCINT20

#define ENCODER2	1
#define OUT2A_PORT	PORTD
#define OUT2B_PORT	PORTD
#define OUT2A_DDR	DDRD
#define OUT2B_DDR	DDRD
#define OUT2A_PIN	PD5
#define OUT2B_PIN	PD6
#define OUT2A_PCINT	PCINT21
#define OUT2B_PCINT	PCINT22

#define ENCODER_RIGHT	ENCODER1
#define ENCODER_LEFT	ENCODER2

#define ENCODER_STOP		0
#define ENCODER_FORWARD		1
#define ENCODER_BACKWARD	2

// Declaration of functions
// Software and hardware initialization
void encoderInit(void);

// Return motor speed in rpm
double encoderGetSpeed(uint8_t encoder);

// Return direction of encoder rotating
uint8_t encoderGetDirection(uint8_t encoder);

// Calculate the motor speed in rpm using a timer with 100ms period
void encoderProcesingData(void);

#endif /* ENCODER_H_ */
