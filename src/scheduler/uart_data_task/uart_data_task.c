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
#include "uart_data_task.h"

/******************************************************************************
*	Variables
******************************************************************************/
char temp;
char tempString[16];
double * pToSettings = 0;

void clearTerminal(void);

// Initialization of stdout
static int uart_putchar(char c, FILE* stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/******************************************************************************
*   Functions
******************************************************************************/
void uartDataTaskInit(void)
{
	// Initialization of communication
	uartInit();
	uartSetBaudRate(115200);
	stdout = &mystdout;
}

void uartDataTask(void)
{
	static unsigned char initStatus = 0;

	if(!initStatus)
	{
		uartDataTaskInit();
		initStatus = 1;
	}

	temp = uartGetByte();

	switch(temp)
	{
	case 'R':
		lineFollowerStart();
		break;
	case 'S':
		lineFollowerStop();
		break;
	case 'W':
		pToSettings = &lineFollowerPID.targetSpeed;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("Set target speed: %s", tempString);
		break;
	case 'P':
		pToSettings = &lineFollowerPID.lineFollowerPidSettings.pGain;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("Set proportional: %s", tempString);
		break;
	case 'I':
		pToSettings = &lineFollowerPID.lineFollowerPidSettings.iGain;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("Set integrative: %s", tempString);
		break;
	case 'D':
		pToSettings = &lineFollowerPID.lineFollowerPidSettings.dGain;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("Set derivative: %s", tempString);
		break;
	case '+':
		*pToSettings += 50;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("parameter = %s", tempString);
		break;
	case '-':
		*pToSettings -= 50;
		dtostrf(*pToSettings, 6, 0, tempString);
		printf("parameter = %s", tempString);
		break;
	}
}

static int uart_putchar(char c, FILE* stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	uartSendByte(c);
	return 0;
}

void clearTerminal(void)
{
	putchar(27);
	printf("[2J");
	putchar(27);
	printf("[H");
}

