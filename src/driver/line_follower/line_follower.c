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

void lineFollowerInit(void)
{
	// Initialization of motor control
//	motorInit();
//	motorStop(RIGHT);
//	motorStop(LEFT);
//	motorSetSpeed(RIGHT, 0, RPM);
//	motorSetSpeed(LEFT, 0, RPM);
	l298Init();
	l298Stop(RIGHT);
	l298Stop(LEFT);
	l298SetPWMDuty(RIGHT, 0);
	l298SetPWMDuty(LEFT, 0);

	// Initialization of line sensor
	lineSenseInit();

	// Initialiation of distance sensors
	distanceSenseInit();

	// Setting the PID parameters
	lineFollowerPID.lineFollowerPidSettings =
	(PidSettings){ I_LINE_MAX, I_LINE_MIN, I_LINE_GAIN, P_LINE_GAIN, D_LINE_GAIN };
	lineFollowerPID.lineFollwerPidState = (PidState){ 0, 0, 0};
	lineFollowerPID.lineFollowerState = LINEF_STOP;
//	lineFollowerPID.targetSpeed = 21000;
//	lineFollowerPID.targetSpeed = 32000;
	lineFollowerPID.targetSpeed = 40000;
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

void lineFollowerUpdatePID(void)
{
	double rightSpeed, leftSpeed, pidTerm;
//	char tempchar2[16];
	unsigned int tempDistance;

	if(lineFollowerPID.lineFollowerState == LINEF_START)
	{

		// Measure the front distance for get obstacle
		tempDistance = distanceSenseGet(SIDE_SENSE, CM_SENSE);
		if(tempDistance < 10 && obstacolDetectat == 0)
		{
			routeObstacle();
			obstacolDetectat = 1;
//			return -1;
		}

		// Get error for followed line
		lineFollowerPID.lineFollwerPidState.error = lineSenseGetError();
		if(lineFollowerPID.lineFollwerPidState.error == -20)
		{
			return;
		}

		pidTerm = updatePID(&lineFollowerPID.lineFollowerPidSettings, &lineFollowerPID.lineFollwerPidState);

		rightSpeed = lineFollowerPID.targetSpeed;
		leftSpeed = lineFollowerPID.targetSpeed;

		if(pidTerm < 0)
		{
			rightSpeed = lineFollowerPID.targetSpeed + pidTerm;
		}
		if(pidTerm > 0)
		{
			leftSpeed = lineFollowerPID.targetSpeed - pidTerm;
		}

		printf("rightSpeed: %d\n", (int16_t)rightSpeed);
		printf("leftSpeed: %d\n", (int16_t)leftSpeed);

//		if(leftSpeed < 0)
//		{
//			leftSpeed *= 2;
//		}
//
//		if(rightSpeed < 0)
//		{
//			rightSpeed *= 2;
//		}

		l298SetPWMDuty(RIGHT, (long)rightSpeed);
		l298Start(RIGHT);
		l298SetPWMDuty(LEFT, (long)leftSpeed);
		l298Start(LEFT);


	} else {
//		motorStop(RIGHT);
//		motorStop(LEFT);
		l298Init();
		l298Stop(RIGHT);
		l298Stop(LEFT);
		l298SetPWMDuty(RIGHT, 0);
		l298SetPWMDuty(LEFT, 0);
		obstacolDetectat = 0;
	}
//	return 0;
}

void routeObstacle(void)
{

	l298SetPWMDuty(RIGHT, 40000);
	l298SetPWMDuty(LEFT, 25000);
	_delay_ms(600);

	l298SetPWMDuty(RIGHT, 40000);
	l298SetPWMDuty(LEFT, 0);
	_delay_ms(600);

	l298SetPWMDuty(RIGHT, 40000);
	l298SetPWMDuty(LEFT, 25000);
	_delay_ms(600);
}
