/*
 * scheduler.h
 *
 *  Created on: Apr 30, 2014
 *      Author: proman
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/******************************************************************************
*	Include files
******************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include "global.h"
#include "hal/timer0/timer0.h"

/******************************************************************************
*	Macro define
******************************************************************************/
#define FULL_LOAD_uC		100
#define MAX_LOAD_uC			FULL_LOAD_uC
#define MAX_NUM_OF_TASKS	6
#define TASK_PERIOD			5000 // us // max. task period 13ms, if need longer task period change clock timer

#define CYCLIC_TIME_0		(unsigned long)TASK_PERIOD // not recomended to use, usage ALL 100% of CPU
#define CYCLIC_TIMEx2		((unsigned long)TASK_PERIOD*2)		// usage 50% of CPU
#define CYCLIC_TIMEx4	 	((unsigned long)TASK_PERIOD*4)		// usage 25% of CPU
#define CYCLIC_TIMEx8		((unsigned long)TASK_PERIOD*8)		// usage 12.5% of CPU
#define CYCLIC_TIMEx16		((unsigned long)TASK_PERIOD*16)		// usage 6.25% of CPU
#define CYCLIC_TIMEx32		((unsigned long)TASK_PERIOD*32)		// usage 3.125% of CPU
#define CYCLIC_TIMEx64 		((unsigned long)TASK_PERIOD*64)		// usage 1.5625% of CPU
#define CYCLIC_TIMEx128		((unsigned long)TASK_PERIOD*128)	// usage 0.78125% of CPU

//#define TIMER_CLOCK			5000000 //Hz //update this with changed timer clock

#define OK		0
#define ERROR	1

/******************************************************************************
*	Variables define
******************************************************************************/
typedef struct schedulerTask
{
	voidFuncPtr		taskFunction;
	unsigned long	taskPriority;
	unsigned long	taskNextTime;
} schedulerTask;

extern schedulerTask taskQueue[MAX_NUM_OF_TASKS];
extern UCHAR tasksInQueue;
extern ULONG schedulerTime;

/******************************************************************************
*	Function define
******************************************************************************/
void schedulerInit(void);
unsigned char schedulerAddTask(voidFuncPtr function, unsigned long priority);
unsigned char schedulerRemoveTask(voidFuncPtr function);
unsigned char schedulerStart(void);
unsigned char schedulerStop(void);
unsigned char schedulerSetTaskPeriod(uint16_t taskPeriod);
void schedulerUpdate(void);
void schedularTick(void);

#endif /* SCHEDULER_H_ */
