/*
 * motor_ctrl.h
 *
 *  Created on: Feb 15, 2014
 *      Author: proman
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#include <avr/io.h>
#include <global.h>
#include <math.h>
#include <hal/timer0/timer0.h>
#include <driver/encoder/encoder.h>
#include <driver/l298/l298.h>
#include <driver/pid_controller/pid_controller.h>

// Period for update of PID controller (*10ms)
#define PID_PERIOD 10

// Wheel diameter in mm
#define WHEEL_DIAM	31.0

// Default coefficients of PID controller
#define P_LM_GAIN	0.6
#define I_LM_GAIN	0
#define D_LM_GAIN	0
#define I_LM_MIN	-500
#define I_LM_MAX	500

#define P_RM_GAIN	0.6
#define I_RM_GAIN	0
#define D_RM_GAIN	0
#define I_RM_MIN	-500
#define I_RM_MAX	500

// Units for speed of motors or wheels
#define RPM		0
#define WRPM	1
#define MPS		2
#define CMPS	3

// Structure for state of motor
typedef struct
{
	signed int currentSpeed;
	signed int targetSpeed;
	char direction;
	char state;
} MotorState;

// General structure for motor
typedef struct
{
	PidSettings motorPidSettings;
	PidState motorPidState;
	MotorState motorState;
} Motor;

/* Function declarations */
/*
 * Initialization of hardware and PID controller
 */
void motorInit(void);

/*
 * Start the motor
 */
void motorStart(char motor);

/*
 * Stop the motor
 */
void motorStop(char motor);

/*
 * Set the direction for motor
 */
void motorSetDirection(char motor, char direction);

/*
 * Get the direction of motor
 */
char motorGetDirection(char motor);

/*
 * Setting target speed for motor
 */
void motorSetSpeed(char motor, double speed, char units);

/*
 * Return current speed of motor
 */
double motorGetSpeed(char motor, char units);

/*
 * Setting the parameter of PID controller for motor
 */
void motorSetPIDState(char motor, PidSettings motorState);

/*
 * Return the structure with settings for PID controller of motor
 */
PidSettings motorGetPIDState(char motor);

/*
 * Funtion for procesing
 */
void motorProcesingSpeed(void);

#endif /* MOTOR_CTRL_H_ */
