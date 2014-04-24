/*
 * l298.h
 *
 *  Created on: Jan 22, 2014
 *      Author: proman
 */

#ifndef L298_H_
#define L298_H_

#include <avr/io.h>
#include <global.h>
#include <util/delay.h>
#include "hal/timer1/timer1.h"

// Configurations of pins
#define DIR1A_PORT	PORTC
#define DIR1B_PORT	PORTC
#define DIR1A_DDR	DDRC
#define DIR1B_DDR	DDRC
#define DIR1A_PIN	PC2
#define DIR1B_PIN	PC3

#define DIR2A_PORT	PORTC
#define DIR2B_PORT	PORTC
#define DIR2A_DDR	DDRC
#define DIR2B_DDR	DDRC
#define DIR2A_PIN	PC4
#define DIR2B_PIN	PC5

// Configurations of PWM pins
#define PWM1_PORT	PORTB
#define PWM2_PORT	PORTB
#define PWM1_DDR	DDRB
#define PWM2_DDR	DDRB
#define PWM1_PIN	PB1
#define PWM2_PIN	PB2

// Configurations of parameters
#define MOTOR1		0
#define MOTOR2		1

#define RIGHT		MOTOR1
#define LEFT		MOTOR2

#define STOP		0
#define FORWARD		1
#define BACKWARD	2

// Declaration of variables
typedef struct
{
	uint8_t status;
	uint8_t direction;
	uint16_t pwmDuty;
} motorParameters;

motorParameters motor1, motor2;

// Declaration of functions
void l298Init(void);

void l298Start(uint8_t _motor);

void l298Stop(uint8_t _motor);

char l298GetStatus(char _motor);

void l298SetPWMDuty(uint8_t _motor, int16_t _duty);

uint16_t l298GetPWMDuty(uint8_t _motor);

#endif /* L298_H_ */
