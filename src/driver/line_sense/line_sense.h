/*
 * line_sense.h
 *
 *  Created on: Feb 25, 2014
 *      Author: proman
 */

#ifndef LINE_SENSE_H_
#define LINE_SENSE_H_

#include <avr/io.h>
#include <global.h>
#include <hal/spi/spi.h>
#include <util/delay.h>

// Voltage reference in Volt
#define V_REF		5.0

#define DDR_CHIP1	DDRD
#define PORT_CHIP1	PORTD
#define PIN_CHIP1	7

#define DDR_CHIP2	DDRB
#define PORT_CHIP2	PORTB
#define PIN_CHIP2	0

void lineSenseInit(void);

double* lineSenseReadVoltage(void);

unsigned char * lineSenseReadLine(void);

signed char lineSenseGetError(void);

#endif /* LINE_SENSE_H_ */
