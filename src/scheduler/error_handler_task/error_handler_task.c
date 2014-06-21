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
#include "error_handler_task.h"

/******************************************************************************
*	Variables
******************************************************************************/
char errorString[16];
unsigned char * line;
double* lineVoltage;

/******************************************************************************
*   Functions
******************************************************************************/
void errorHandlerTaskInit(void)
{

}

void errorHandlerTask(void)
{
	static unsigned char initStatus = 0;

	if(!initStatus)
	{
		errorHandlerTaskInit();
		initStatus = 1;
	}

	// For debug uncomment necessary line

	// Debug distance from distnace sensor
//	printf("Distance: %d\n", distanceSenseGet(SIDE_SENSE, CM_SENSE));

	// Debug line sensor voltages
//	lineVoltage = lineSenseReadVoltage();
//	for(int i = 0; i < 16; i++)
//	{
//		dtostrf(lineVoltage[i], 5, 2, errorString);
//		printf("%s, ", errorString);
//	}
//	printf("\n");

	// Debug line sensor readed line
//	line = lineSenseReadLine();
//	for(int i = 0; i < 16; i++)
//	{
//		printf("%d", line[i]);
//	}
//	printf("\n");

	// Debug line sensor calculated error
//	printf("Error: %d\n", lineSenseGetError());
}
