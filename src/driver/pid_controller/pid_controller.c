/*
 * pid_controller.c
 *
 *  Created on: Apr 17, 2014
 *      Author: proman
 */
#include "pid_controller.h"

double updatePID(PidSettings * pidSettings, PidState * pidState)
{
	double pTerm, dTerm, iTerm;

	pTerm = pidSettings->pGain * pidState->error;    // calculate the proportional term

	pidState->iState += pidState->error;          // calculate the integral state with appropriate limiting
	if (pidState->iState > pidSettings->iMax)
		pidState->iState = pidSettings->iMax;
	else if (pidState->iState < pidSettings->iMin)
		pidState->iState = pidSettings->iMin;
	iTerm = pidSettings->iGain * pidState->iState;    // calculate the integral term

	dTerm = pidSettings->dGain * (pidState->error - pidState->dState);
	pidState->dState = pidState->error;

	return (pTerm + iTerm + dTerm);
}
