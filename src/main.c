#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

#include "driver/encoder/encoder.h"
#include "driver/motor_ctrl/motor_ctrl.h"
#include "driver/l298/l298.h"
#include "driver/distance_sense/distance_sense.h"
#include "driver/line_sense/line_sense.h"
#include "driver/line_follower/line_follower.h"

#include "scheduler/scheduler.h"
#include "scheduler/led_task/led_task.h"
#include "scheduler/error_handler_task/error_handler_task.h"
#include "scheduler/uart_data_task/uart_data_task.h"

int main(void)
{
	// Send message
	printf("Hello World!!!\n");

	// Initialization of distance sensor module
//	distanceSenseInit();

	// Initialization of line sensor module
//	lineSenseInit();

	// Initialization of l298 driver and stop motors
//	encoderInit();
//	l298Init();
//	l298Stop(RIGHT);
//	l298Stop(LEFT);
//	l298SetPWMDuty(RIGHT, 0);
//	l298SetPWMDuty(LEFT, 0);

	// Initialization of motor control module
//	motorInit();

	// Initialization of lineFollower module
//	lineFollowerInit();
//	lineFollowerStart();

	// Initialization of scheduler module and adding task for execution
//	schedulerInit();
	// Tasks using for lineFollower application
//	schedulerAddTask(&encoderProcesingData, CYCLIC_TIMEx8);
//	schedulerAddTask(&motorProcesingSpeed, CYCLIC_TIMEx8);
//	schedulerAddTask(&lineFollowerUpdatePID, CYCLIC_TIMEx2);
	// Tasks for system using (for info see tasks in /scheduler/tasks)
//	schedulerAddTask(&uartDataTask, CYCLIC_TIMEx8);
//	schedulerAddTask(&errorHandlerTask, CYCLIC_TIMEx8);
//	schedulerAddTask(&ledTask, CYCLIC_TIMEx128);
//	schedulerStart();

	while(1)
	{
//		schedulerUpdate();
	}
}
