/*
 * motor_ctrl.c
 *
 *  Created on: Feb 15, 2014
 *      Author: proman
 */

#include "motor_ctrl.h"

Motor motorRight;
Motor motorLeft;
uint8_t periodCounter;

void motorInit(void)
{
	// Variable initialization
	motorRight.motorPidSettings.pGain = P_RM_GAIN;
	motorRight.motorPidSettings.iGain = I_RM_GAIN;
	motorRight.motorPidSettings.dGain = D_RM_GAIN;
	motorRight.motorPidSettings.iMin = I_RM_MIN;
	motorRight.motorPidSettings.iMax = I_RM_MAX;
	motorRight.motorPidState = (PidState){0, 0, 0};
	motorRight.motorState = (MotorState){0, 0, 0, 0};
	motorRight.motorState.state = STOP;
	motorRight.motorState.direction = FORWARD;

	motorLeft.motorPidSettings.pGain = P_LM_GAIN;
	motorLeft.motorPidSettings.iGain = I_LM_GAIN;
	motorLeft.motorPidSettings.dGain = D_LM_GAIN;
	motorLeft.motorPidSettings.iMin = I_LM_MIN;
	motorLeft.motorPidSettings.iMax = I_LM_MAX;
	motorLeft.motorPidState = (PidState){0, 0, 0};
	motorLeft.motorState = (MotorState){0, 0, 0, 0};
	motorLeft.motorState.state = STOP;
	motorLeft.motorState.direction = FORWARD;

	// Hardware initialization
	l298Init();
	l298SetPWMDuty(MOTOR1, 0);
	l298SetPWMDuty(MOTOR2, 0);
	encoderInit();

}

void motorStart(char motor)
{
	switch (motor) {
		case RIGHT:
			motorRight.motorState.state = motorRight.motorState.direction;
			break;
		case LEFT:
			motorLeft.motorState.state = motorLeft.motorState.direction;
			break;
		default:
			break;
	}
}

void motorStop(char motor)
{
	switch (motor) {
		case RIGHT:
			motorRight.motorState.state = STOP;
			break;
		case LEFT:
			motorLeft.motorState.state = STOP;
			break;
		default:
			break;
	}
}

char motorGetStatus(char motor)
{
	switch (motor) {
		case RIGHT:
			return motorRight.motorState.state;
			break;
		case LEFT:
			return motorLeft.motorState.state;
			break;
		default:
			break;
	}
	return -1;
}

void motorSetDirection(char motor, char direction)
{
	switch (motor) {
		case RIGHT:
			motorRight.motorState.direction = direction;
			break;
		case LEFT:
			motorLeft.motorState.direction = direction;
			break;
		default:
			break;
	}
}

char motorGetDirection(char motor)
{
	switch (motor) {
		case RIGHT:
			return motorRight.motorState.direction;
			break;
		case LEFT:
			return motorLeft.motorState.direction;
			break;
		default:
			break;
	}
	return -1;
}

void motorSetSpeed(char motor, double speed, char units)
{
	if(speed > 30000)
	{
		speed = 30000;
	}
	if(speed < -30000)
	{
		speed = -30000;
	}
	switch (units) {
		case RPM:
			speed = 200.0 * round(speed / 200.0);
			break;
		case WRPM:
			speed = speed * 30; // rate of reduction is 30
			speed = 200.0 * round(speed / 200.0);
			break;
		case MPS:
			speed = (1800000.0 * speed) / (M_PI * WHEEL_DIAM);
			speed = 200.0 * round(speed / 200.0);
			break;
		case CMPS:
			speed = (180000000.0 * speed) / (M_PI * WHEEL_DIAM);
			speed = 200.0 * round(speed / 200.0);
			break;
		default:
			break;
	}
	switch (motor) {
		case RIGHT:
			motorRight.motorState.targetSpeed = speed;
			break;
		case LEFT:
			motorLeft.motorState.targetSpeed = speed;
			break;
		default:
			break;
	}
}

double motorGetSpeed(char motor, char units)
{
	double speed = 0;
	switch (motor) {
		case RIGHT:
			speed = motorRight.motorState.targetSpeed;
			break;
		case LEFT:
			speed = motorLeft.motorState.targetSpeed;
			break;
		default:
			break;
	}
	switch (units) {
		case RPM:
			// do nothing
			break;
		case WRPM:
			speed = speed / 30;
			break;
		case MPS:
			speed = (speed * M_PI * WHEEL_DIAM) / 1800000.0;
			break;
		case CMPS:
			speed = (speed * M_PI * WHEEL_DIAM) / 180000000.0;
			break;
		default:
			break;
	}
	return speed;
}

void motorSetPIDState(char motor, PidSettings motorState)
{
	switch (motor) {
		case RIGHT:
			motorRight.motorPidSettings = motorState;
			break;
		case LEFT:
			motorLeft.motorPidSettings = motorState;
			break;
		default:
			break;
	}
}

PidSettings motorGetPIDState(char motor)
{
	switch (motor) {
		case RIGHT:
			return motorRight.motorPidSettings;
			break;
		case LEFT:
			return motorLeft.motorPidSettings;
			break;
		default:
			break;
	}
	return (PidSettings){0, 0, 0, 0};
}

// call this task every 50ms
void motorProcesingSpeed(void)
{
	double rightSpeed, leftSpeed;

	motorRight.motorPidState.error = motorRight.motorState.targetSpeed - encoderGetSpeed(ENCODER_RIGHT);
	motorLeft.motorPidState.error = motorLeft.motorState.targetSpeed - encoderGetSpeed(ENCODER_LEFT);

	if(motorRight.motorState.state != STOP)
	{
		rightSpeed = updatePID(&motorRight.motorPidSettings, &motorRight.motorPidState);
		rightSpeed = l298GetPWMDuty(RIGHT) + rightSpeed;
		l298SetPWMDuty(RIGHT, (uint16_t)(rightSpeed));
		if(l298GetStatus(RIGHT) == STOP)
		{
			l298Start(RIGHT);
		}
	} else if(l298GetStatus(RIGHT) != STOP)
	{
		l298Stop(RIGHT);
		motorRight.motorPidState = (PidState){0, 0, 0};
	}

	if(motorLeft.motorState.state != STOP)
	{
		leftSpeed = updatePID(&motorLeft.motorPidSettings, &motorLeft.motorPidState);
		leftSpeed = l298GetPWMDuty(LEFT) + leftSpeed;
		l298SetPWMDuty(LEFT, (uint16_t)(leftSpeed));
		if(l298GetStatus(LEFT) == STOP)
		{
			l298Start(LEFT);
		}
	} else if(l298GetStatus(LEFT) != STOP)
	{
		l298Stop(LEFT);
		motorLeft.motorPidState = (PidState){0, 0, 0};
	}

}
