/*
 * scheduler.c
 *
 *  Created on: Apr 30, 2014
 *      Author: proman
 */

/******************************************************************************
*   Include files
******************************************************************************/
#include "scheduler.h"

/******************************************************************************
*	Variables
******************************************************************************/
schedulerTask taskQueue[MAX_NUM_OF_TASKS];
UCHAR tasksInQueue;
ULONG schedulerTime;
voidFuncPtr nextRunFunction = 0;

/******************************************************************************
*   Functions
******************************************************************************/
void schedulerInit(void)
{
	tasksInQueue = 0;
	schedulerTime = 0;

	// Initialize timer0 for speed control
	timer0Init(TIMER_CLK_DIV1024);
	timer0CTCInit();
	timer0CTCSetPeriod(TASK_PERIOD);// in us
}
/* This function add the task in schedular queue.
**   Parameters:
**	@function - function reference for task
** @priority - task priority in schedular
**   Returns:
**	@STATUS - status of operation
*/
unsigned char schedulerAddTask(voidFuncPtr function, UCHAR priority)
{
	unsigned char status = ERROR;
	schedulerTask tempTask;
	UCHAR i, count;

	// calculate the uC loading
	USHORT ucLoad = FULL_LOAD_uC/(priority/TASK_PERIOD);
	for(i = 0; i < tasksInQueue; i++)
	{
		ucLoad += FULL_LOAD_uC/(taskQueue[i].taskPriority/TASK_PERIOD);
	}

	if((ucLoad <= MAX_LOAD_uC) && (tasksInQueue+1 <= MAX_NUM_OF_TASKS))
	{
		taskQueue[tasksInQueue].taskFunction = function;
		taskQueue[tasksInQueue].taskPriority = priority;
		taskQueue[tasksInQueue].taskNextTime = 0;
		tasksInQueue++;
		status = OK;
	}
	else
	{
		return ERROR;
	}

	do // ordering tasks in funtions of priority
	{
		count = 0;
		for(i = 0; i < tasksInQueue-1; i++)
		{
			if(taskQueue[i].taskPriority > taskQueue[i+1].taskPriority)
			{
				tempTask = taskQueue[i];
				taskQueue[i] = taskQueue[i+1];
				taskQueue[i+1] = tempTask;
				count++;
			}
		}
	} while(count);

	// reseting time of tasks
	for(i = 0; i < tasksInQueue; i++)
	{
		taskQueue[i].taskNextTime = 0;
	}

	return status;
}

/* This function stop the specific task in schedular queue.
**   Parameters:
**	@function - function reference of task
**   Returns:
**	@STATUS - status of operation
*/
unsigned char schedulerRemoveTask(voidFuncPtr function)
{
	unsigned char status = ERROR;
	schedulerTask tempTask;
	UCHAR i, j, count;

	for(i = 0; i < tasksInQueue; i++)
	{
		if(taskQueue[i].taskFunction == function)
		{
			for(j = i+1; j < tasksInQueue; j++)
			{
				taskQueue[i] = taskQueue[j];
				i++;
			}
			tasksInQueue--;
			status = OK;
		}
	}

	do
	{
		count = 0;
		for(i = 0; i < tasksInQueue-1; i++)
		{
			if(taskQueue[i].taskPriority > taskQueue[i+1].taskPriority)
			{
				tempTask = taskQueue[i];
				taskQueue[i] = taskQueue[i+1];
				taskQueue[i+1] = tempTask;
				count++;
			}
		}
	} while(count);

	for(i = 0; i < tasksInQueue; i++)
	{
		taskQueue[i].taskNextTime = 0;
	}

	return status;
}

/* This function start the schedular.
**   Parameters:
**	none
**   Returns:
**	@STATUS - status of operation
*/
unsigned char schedulerStart(void)
{
	unsigned char status = ERROR;
	timer0Attach(TIMER0_OUTCOMPAREA_INT, &schedularTick);
	status = OK;
	return status;
}

/* This function stop the schedular.
**   Parameters:
**	none
**   Returns:
**	@STATUS - status of operation
*/
unsigned char schedulerStop(void)
{
	unsigned char status = ERROR;
	timer0Detach(TIMER0_OUTCOMPAREA_INT, &schedularTick);
	status = OK;
	return status;
}

/* This function set the period for a task in queue.
**   Parameters:
**	@taskPeriod in ms
**   Returns:
**	@STATUS - status of operation
*/
unsigned char schedulerSetTaskPeriod(uint16_t taskPeriod)
{
	timer0CTCSetPeriod(taskPeriod); // in us
	return OK;
}

/* This function check if is nedd to run one of task in queue.
**   Parameters:
**	none
**   Returns:
**	none
*/
void schedulerUpdate(void)
{
	if(nextRunFunction == 0)
	{

	} else {
		nextRunFunction();
		nextRunFunction = 0;
	}
}

/* This function is interrupt for procesing schedular queue.
**   Parameters:
**	none
**   Returns:
**	none
*/
void schedularTick(void)
{
	UCHAR i;
	schedulerTime += TASK_PERIOD;
	for(i = 0; i < tasksInQueue; i++)
	{
		if(taskQueue[i].taskNextTime == schedulerTime)
		{
			//taskQueue[i].taskFunction();
			nextRunFunction = taskQueue[i].taskFunction;
			taskQueue[i].taskNextTime += taskQueue[i].taskPriority;
			return;
		}
	}
	for(i = 0; i < tasksInQueue; i++)
	{
		if(0UL == taskQueue[i].taskNextTime)
		{
			//taskQueue[i].taskFunction();
			nextRunFunction = taskQueue[i].taskFunction;
			taskQueue[i].taskNextTime = schedulerTime + taskQueue[i].taskPriority;
			return;
		}
	}
	return;
}
