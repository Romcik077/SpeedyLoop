/*
 * pid_controller.h
 *
 *  Created on: Apr 17, 2014
 *      Author: proman
 */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

// Structure for state of PID controller
typedef struct PidState
{
	double dState;		// Last position input
	double iState;		// Integrator state
	double error;
} PidState;

// Structure for settings of PID controller
typedef struct PidSettings
{
	double iMax, iMin;	// Maximum and minimum allowable integrator state
	double iGain,		// integral gain
		   pGain,		// proportional gain
		   dGain;		// derivative gain
} PidSettings;

double updatePID(PidSettings * pidSettings, PidState * pidState);

#endif /* PID_CONTROLLER_H_ */
