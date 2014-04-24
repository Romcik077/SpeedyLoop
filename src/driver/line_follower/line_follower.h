/*
 * line_follower.h
 *
 *  Created on: Feb 28, 2014
 *      Author: proman
 */

#ifndef LINE_FOLLOWER_H_
#define LINE_FOLLOWER_H_

#include <avr/io.h>
#include <global.h>
#include <driver/motor_ctrl/motor_ctrl.h>
#include <driver/line_sense/line_sense.h>
#include <driver/l298/l298.h>
#include "driver/distance_sense/distance_sense.h"

#define LINEF_START	0
#define LINEF_STOP	1

// Default coefficients of PID controller
#define P_LINE_GAIN 700
#define I_LINE_GAIN	0
#define D_LINE_GAIN	3000
#define I_LINE_MIN	-3000
#define I_LINE_MAX	3000

typedef struct
{
	PidSettings lineFollowerPidSettings;
	PidState lineFollwerPidState;
	unsigned char lineFollowerState;
	double targetSpeed;
}LineFollower;

extern LineFollower lineFollowerPID;

void lineFollowerInit(void);

void lineFollowerStart(void);

void lineFollowerStop(void);

void lineFollowerSetSpeed(double speed, char units);

double lineFollowerGetSpeed(char units);

signed char lineFollowerUpdatePID(void);

#endif /* LINE_FOLLOWER_H_ */
