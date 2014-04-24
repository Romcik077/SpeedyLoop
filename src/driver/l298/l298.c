/*
 * l298.c
 *
 *  Created on: Jan 22, 2014
 *      Author: proman
 */
#include "l298.h"

void l298Init(void)
{
	// Init DIR pins as output
	sbi(DIR1A_DDR, DIR1A_PIN);
	sbi(DIR1B_DDR, DIR1B_PIN);
	sbi(DIR2A_DDR, DIR2A_PIN);
	sbi(DIR2B_DDR, DIR2B_PIN);

	// Init PWM pins as output
	sbi(PWM1_DDR, PWM1_PIN);
	sbi(PWM2_DDR, PWM2_PIN);

	// Clear output pins
	cbi(DIR1A_PORT, DIR1A_PIN);
	cbi(DIR1B_PORT, DIR1B_PIN);
	cbi(DIR2A_PORT, DIR2A_PIN);
	cbi(DIR2B_PORT, DIR2B_PIN);

	// Init timers for PWM
	// Set non-inverting mode of output pins, waveform generation FastPWM(16bit, TOP ICR1)
	timer1Init(TIMER_CLK_DIV1);
	timer1PWMInitICR(45535);
	timer1PWMASet(0);
	timer1PWMBSet(0);
	timer1PWMAOn();
	timer1PWMBOn();

	// Initialization of variables for motors
	motor1.status = STOP;
	motor1.pwmDuty = 0;
	motor1.direction = FORWARD;

	motor2.status = STOP;
	motor2.pwmDuty = 0;
	motor2.direction = FORWARD;
}

void l298Start(uint8_t _motor)
{
	switch (_motor) {
		case MOTOR1:
			switch (motor1.direction) {
				case FORWARD:
					timer1PWMASet(motor1.pwmDuty);
					cbi(DIR1A_PORT, DIR1A_PIN);
					sbi(DIR1B_PORT, DIR1B_PIN);
					motor1.status = FORWARD;
					break;
				case BACKWARD:
					timer1PWMASet(motor1.pwmDuty);
					sbi(DIR1A_PORT, DIR1A_PIN);
					cbi(DIR1B_PORT, DIR1B_PIN);
					motor1.status = BACKWARD;
					break;
				default:
					break;
			}
			break;
		case MOTOR2:
			switch (motor2.direction) {
				case FORWARD:
					timer1PWMBSet(motor2.pwmDuty);
					sbi(DIR2A_PORT, DIR2A_PIN);
					cbi(DIR2B_PORT, DIR2B_PIN);
					motor2.status = FORWARD;
					break;
				case BACKWARD:
					timer1PWMBSet(motor2.pwmDuty);
					cbi(DIR2A_PORT, DIR2A_PIN);
					sbi(DIR2B_PORT, DIR2B_PIN);
					motor2.status = BACKWARD;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void l298Stop(uint8_t _motor)
{
	switch (_motor) {
		case MOTOR1:
			cbi(DIR1A_PORT, DIR1A_PIN);
			cbi(DIR1B_PORT, DIR1B_PIN);
			motor1.status = STOP;
			break;
		case MOTOR2:
			cbi(DIR2A_PORT, DIR2A_PIN);
			cbi(DIR2B_PORT, DIR2B_PIN);
			motor2.status = STOP;
			break;
		default:
			break;
	}
}

char l298GetStatus(char _motor)
{
	switch (_motor) {
		case MOTOR1:
			return motor1.status;
			break;
		case MOTOR2:
			return motor2.status;
			break;
		default:
			break;
	}
	return -1;
}

void l298SetPWMDuty(uint8_t _motor, int16_t _duty)
{
	switch (_motor) {
		case MOTOR1:
			if(_duty < 0)
			{
				_duty *= -1;
				motor1.direction = BACKWARD;
			} else {
				motor1.direction = FORWARD;
			}

			if(_duty > 65000)
				_duty = 65000;

			motor1.pwmDuty = _duty;

			if(motor1.status != STOP)
				l298Start(MOTOR1);

			break;
		case MOTOR2:
			if(_duty < 0)
			{
				_duty *= -1;
				motor2.direction = BACKWARD;
			} else {
				motor2.direction = FORWARD;
			}

			if(_duty > 65000)
				_duty = 65000;

			motor2.pwmDuty = _duty;

			if(motor2.status != STOP)
				l298Start(MOTOR2);

			break;
		default:
			break;
	}
}

uint16_t l298GetPWMDuty(uint8_t _motor)
{
	switch (_motor) {
		case MOTOR1:
			return motor1.pwmDuty;
			break;
		case MOTOR2:
			return motor2.pwmDuty;
			break;
		default:
			break;
	}
	return 0;
}
