/******************************************************************************
*	File-name:		test_task.h
*	Version:		v0.1
*	Project:		SpeedyLoop
*	Device:			ATmega328p
*	IDE/Compiler:	Eclipse c++/AVR GCC Tollchain
*	Description:	Test task
*	Author:			Roman Panuta
*	History:
*	   v0.1: Generating, editing and adding this file in project
******************************************************************************/

#ifndef _ERROR_HANDLER_TASK_H_
#define _ERROR_HANDLER_TASK_H_

/******************************************************************************
*	Include files
******************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "driver/distance_sense/distance_sense.h"
#include "driver/line_sense/line_sense.h"
#include "driver/line_follower/line_follower.h"

/******************************************************************************
*	Macro define
******************************************************************************/


/******************************************************************************
*	Variables define
******************************************************************************/


/******************************************************************************
*	Function define
******************************************************************************/
void errorHandlerTaskInit(void);
void errorHandlerTask(void);


#endif
