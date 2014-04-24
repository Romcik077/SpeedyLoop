#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "hal/uart/uart.h"
//#include "driver/motor_ctrl/motor_ctrl.h"
#include "driver/l298/l298.h"
#include "driver/distance_sense/distance_sense.h"
#include "driver/line_sense/line_sense.h"
#include "driver/line_follower/line_follower.h"

#define LED1_DDR	DDRD
#define LED1_PORT	PORTD
#define LED1_PIN	PD2

void clearTerminal(void);

// Initialization of stdout
static int uart_putchar(char c, FILE* stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	char temp;
	char tempString[16];
	unsigned char * line;
	double * pToSettings = 0;

	sbi(DDRD, PD2);

	// Initialization of communication
	uartInit();
	uartSetBaudRate(115200);
	stdout = &mystdout;

	// Send message
	printf("Hello World!!!\n");
	_delay_ms(500);

	// Initialization of distance sensor module
	distanceSenseInit();

	// Initialization of line sensor module
	//lineSenseInit();

	// Initialization of motor control module
	//motorInit();

	lineFollowerInit();
//	lineFollowerStart();

	while(1)
	{
//		clearTerminal();
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

		printf("Distance: %d\n", distanceSenseGet(FRONT_SENSE, CM_SENSE));

//		if(distanceSenseGet(FRONT_SENSE, CM_SENSE) < 10)
//		{
//			routeObstacle();
//		}

//		line = lineSenseReadLine();
//		for(int i = 0; i < 16; i++)
//		{
//			printf("%d", line[i]);
//		}
//		printf("\n");

		if(lineFollowerUpdatePID() == -1)
		{
			// TODO return to line
		}
		_delay_ms(10);
	}
}

void clearTerminal(void)
{
	putchar(27);
	printf("[2J");
	putchar(27);
	printf("[H");
}

static int uart_putchar(char c, FILE* stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	uartSendByte(c);
	return 0;
}
