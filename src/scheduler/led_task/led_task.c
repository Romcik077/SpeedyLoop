/******************************************************************************
*	File-name:		test_task.c
*	Version:		v0.1
*	Project:		SpeedyLoop
*	Device:			ATmega328p
*	IDE/Compiler:	Eclipse c++/AVR GCC Tollchain
*	Description:	Test task
*	Author:			Roman Panuta
*	History:
*	   v0.1: Generating, editing and adding this file in project
******************************************************************************/

/******************************************************************************
*   Include files
******************************************************************************/
#include "led_task.h"

/******************************************************************************
*	Variables
******************************************************************************/


/******************************************************************************
*   Functions
******************************************************************************/
void ledTaskInit(void)
{
	sbi(DDRD, PD2);
}

void ledTask(void)
{
	static unsigned char initStatus = 0;

	if(!initStatus)
	{
		ledTaskInit();
		initStatus = 1;
	}

	if(bit_is_set(PIND, PD2))
		cbi(PORTD, PD2);
	else
		sbi(PORTD, PD2);
}
