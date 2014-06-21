/*
 * distance_sense.h
 *
 *  Created on: Feb 24, 2014
 *      Author: proman
 */

#ifndef DISTANCE_SENSE_H_
#define DISTANCE_SENSE_H_

#include <avr/io.h>
#include <global.h>
#include <hal/a2d/a2d.h>
#include <avr/pgmspace.h>

#define FRONT_SENSE 6
#define SIDE_SENSE	7

#define MM_SENSE	0
#define CM_SENSE	1

#define tableStartDistance 5
#define tableEndDistance 80

void distanceSenseInit(void);

unsigned int distanceSenseGet(unsigned char sense, unsigned char units);

#endif /* DISTANCE_SENSE_H_ */
