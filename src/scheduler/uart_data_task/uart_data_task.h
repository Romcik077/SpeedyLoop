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

#ifndef _UART_DATA_TASK_H_
#define _UART_DATA_TASK_H_

/******************************************************************************
*	Include files
******************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "hal/uart/uart.h"
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
void uartDataTaskInit(void);
void uartDataTask(void);


#endif
