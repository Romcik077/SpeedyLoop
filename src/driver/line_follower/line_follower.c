/*
 * line_follower.c
 *
 *  Created on: Feb 28, 2014
 *      Author: proman
 */

#include "line_follower.h"
#include <stdio.h>
#include <stdlib.h>

LineFollower lineFollowerPID;
unsigned char obstacolDetectat = 0;
double time = 0;
unsigned char timerPornit = 0;

void routeObstacle(void);
void timeProcesing(void);

void lineFollowerInit(void)
{
	// Initialization of motor control
//	motorInit();
//	motorStop(MOTOR1);
//	motorStop(MOTOR2);
//	motorSetSpeed(MOTOR1, 0, RPM);
//	motorSetSpeed(MOTOR2, 0, RPM);
	l298Init();
	l298Stop(RIGHT);
	l298Stop(LEFT);
	l298SetPWMDuty(RIGHT, 0);
	l298SetPWMDuty(LEFT, 0);

	// Initialization of line sensor
	lineSenseInit();

	distanceSenseInit();

	// Initialize timer0 for speed control
	timer0Init(TIMER_CLK_DIV1024);
	timer0CTCInit();
	timer0CTCSetPeriod(20);
//	timer0Attach(TIMER0_OUTCOMPAREA_INT, timeProcesing);

	// Setting the PID parameters
	lineFollowerPID.lineFollowerPidSettings =
	(PidSettings){ I_LINE_MAX, I_LINE_MIN, I_LINE_GAIN, P_LINE_GAIN, D_LINE_GAIN };
	lineFollowerPID.lineFollwerPidState = (PidState){ 0, 0, 0};
	lineFollowerPID.lineFollowerState = LINEF_STOP;
	lineFollowerPID.targetSpeed = 23000;
}

void lineFollowerStart(void)
{
	lineFollowerPID.lineFollowerState = LINEF_START;
}

void lineFollowerStop(void)
{
	lineFollowerPID.lineFollowerState = LINEF_STOP;
}

void lineFollowerSetSpeed(double speed, char units)
{
//	switch (units) {
//		case RPM:
//			speed = 200.0 * round(speed / 200.0);
//			break;
//		case WRPM:
//			speed = speed * 30; // rate of reduction is 30
//			speed = 200.0 * round(speed / 200.0);
//			break;
//		case MPS:
//			speed = (1800000.0 * speed) / (M_PI * WHEEL_DIAM);
//			speed = 200.0 * round(speed / 200.0);
//			break;
//		case CMPS:
//			speed = (180000000.0 * speed) / (M_PI * WHEEL_DIAM);
//			speed = 200.0 * round(speed / 200.0);
//			break;
//		default:
//			break;
//	}
	lineFollowerPID.targetSpeed = speed;
}

double lineFollowerGetSpeed(char units)
{
	double speed = 0;
	speed = lineFollowerPID.targetSpeed;
//	switch (units) {
//		case RPM:
//			// do nothing
//			break;
//		case WRPM:
//			speed = speed / 30;
//			break;
//		case MPS:
//			speed = (speed * M_PI * WHEEL_DIAM) / 1800000.0;
//			break;
//		case CMPS:
//			speed = (speed * M_PI * WHEEL_DIAM) / 180000000.0;
//			break;
//		default:
//			break;
//	}
	return speed;
}

signed char lineFollowerUpdatePID(void)
{
	double rightSpeed, leftSpeed, pidTerm;
//	char tempchar2[16];
	unsigned int tempDistance;

	if(lineFollowerPID.lineFollowerState == LINEF_START)
	{
		// Attach interrupt for calculate time
		if(timerPornit == 0)
		{
			timer0Attach(TIMER0_OUTCOMPAREA_INT, timeProcesing);
			timerPornit = 1;
		}

		// Measure the front distance for get obstacle
		tempDistance = distanceSenseGet(FRONT_SENSE, CM_SENSE);
		if(tempDistance < 25 && obstacolDetectat == 0)
		{
			routeObstacle();
			obstacolDetectat = 1;
			lineFollowerPID.targetSpeed = 30000;
			return -1;
		}

		// Get error for followed line
		lineFollowerPID.lineFollwerPidState.error = lineSenseGetError();

		pidTerm = updatePID(&lineFollowerPID.lineFollowerPidSettings, &lineFollowerPID.lineFollwerPidState);

		rightSpeed = lineFollowerPID.targetSpeed;
		leftSpeed = lineFollowerPID.targetSpeed;

		if(lineFollowerPID.lineFollwerPidState.error < 0)
		{
			rightSpeed /= (lineFollowerPID.lineFollwerPidState.error * -1)/1.5;
			leftSpeed /= (lineFollowerPID.lineFollwerPidState.error * -1)/1.5;
		}
		if(lineFollowerPID.lineFollwerPidState.error > 0) {
			rightSpeed /= (lineFollowerPID.lineFollwerPidState.error)/1.5;
			leftSpeed /= (lineFollowerPID.lineFollwerPidState.error)/1.5;
		}

//		dtostrf(pidTerm, 6, 2, tempchar2);
//		printf("pidTerm: %s\n", tempchar2);

		if(pidTerm < 0)
		{
			rightSpeed += pidTerm*2;
			leftSpeed -= pidTerm;
		}
		if(pidTerm > 0)
		{
			rightSpeed += pidTerm;
			leftSpeed -= pidTerm*2;
		}

//		rightSpeed += pidTerm;
//		leftSpeed -= pidTerm;

//		dtostrf(rightSpeed, 6, 2, tempchar2);
//		printf("rightSpeed: %s\n", tempchar2);
//		dtostrf(leftSpeed, 6, 2, tempchar2);
//		printf("leftSpeed: %s\n", tempchar2);

		if(leftSpeed < 0)
		{
			leftSpeed *= -1;
			leftSpeed *= 1.4;
			if(leftSpeed > 65000)
				leftSpeed = 65000;
			l298Start(LEFT);
			l298SetDirection(LEFT, BACKWARD);
			l298SetPWMDuty(LEFT, leftSpeed);
		} else {
			if(leftSpeed > 65000)
				leftSpeed = 65000;
			l298Start(LEFT);
			l298SetDirection(LEFT, FORWARD);
			l298SetPWMDuty(LEFT, leftSpeed);
		}

		if(rightSpeed < 0)
		{
			rightSpeed *= -1;
			rightSpeed *= 1.4;
			if(rightSpeed > 65000)
				rightSpeed = 65000;
			l298Start(RIGHT);
			l298SetDirection(RIGHT, BACKWARD);
			l298SetPWMDuty(RIGHT, rightSpeed);
		} else {
//			rightSpeed *= 2.5;
			if(rightSpeed > 65000)
				rightSpeed = 65000;
			l298Start(RIGHT);
			l298SetDirection(RIGHT, FORWARD);
			l298SetPWMDuty(RIGHT, rightSpeed);
		}


	} else {
		l298Init();
		l298Stop(RIGHT);
		l298Stop(LEFT);
		l298SetPWMDuty(RIGHT, 0);
		l298SetPWMDuty(LEFT, 0);
		timer0Detach(TIMER0_OUTCOMPAREA_INT);
		time = 0;
		obstacolDetectat = 0;
	}
	return 0;
}

void routeObstacle(void)
{
//	l298SetPWMDuty(RIGHT, 0);
//	l298SetPWMDuty(LEFT, 0);
//	_delay_ms(2000);

	l298SetDirection(RIGHT, BACKWARD);
	l298SetDirection(LEFT, BACKWARD);
	l298SetPWMDuty(RIGHT, 5000);
	l298SetPWMDuty(LEFT, 5000);
	_delay_ms(400);

	l298SetDirection(RIGHT, FORWARD);
	l298SetDirection(LEFT, FORWARD);
	l298SetPWMDuty(RIGHT, 25000);
	l298SetPWMDuty(LEFT, 0);
	_delay_ms(250);
	l298SetPWMDuty(RIGHT, 12000);
	l298SetPWMDuty(LEFT, 12000);
	_delay_ms(350);
	l298SetPWMDuty(RIGHT, 0);
	l298SetPWMDuty(LEFT, 20000);
	_delay_ms(700);
	l298SetPWMDuty(RIGHT, 12000);
	l298SetPWMDuty(LEFT, 12000);
	_delay_ms(600);
	l298SetPWMDuty(RIGHT, 12000);
	l298SetPWMDuty(LEFT, 0);
	_delay_ms(300);

//	while(distanceSenseGet(SIDE_SENSE, CM_SENSE) < 10);

//	l298SetPWMDuty(RIGHT, 0);
//	l298SetPWMDuty(LEFT, 12000);
//	_delay_ms(500);
//	l298SetPWMDuty(RIGHT, 12000);
//	l298SetPWMDuty(LEFT, 12000);
//	_delay_ms(500);
//	l298SetPWMDuty(RIGHT, 12000);
//	l298SetPWMDuty(LEFT, 0);
//	_delay_ms(500);
//	l298SetPWMDuty(RIGHT, 12000);
//	l298SetPWMDuty(LEFT, 12000);

}

void timeProcesing(void)
{
	time += 0.01;
}
