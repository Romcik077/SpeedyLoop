/*
 * distance_sense.c
 *
 *  Created on: Feb 24, 2014
 *      Author: proman
 */

#include "distance_sense.h"

// 75 elemnts of table
const uint16_t __attribute__ ((progmem)) distanceVoltageValue[] =
{
	3080, 3070, 3050, 2950,
	2640, 2420, 2220, 2030,
	1870, 1730, 1650, 1540,
	1450, 1375, 1310, 1245,
	1210, 1130, 1105, 1050,
	1110, 970, 940, 900,
	880, 850, 825, 805,
	780, 765, 745, 725,
	705, 685, 665, 630,
	620, 610, 600, 580,
	560, 550, 540, 530,
	520, 510, 505, 500,
	495, 490, 485, 480,
	475, 470, 465, 460,
	455, 450, 445, 440,
	435, 430, 425, 420,
	415, 410, 405, 400,
	395, 390, 385, 380,
	375, 370, 355
};

void distanceSenseInit(void)
{
	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV64);
	a2dSetReference(ADC_REFERENCE_AREF);
}

unsigned int distanceSenseGet(unsigned char sense, unsigned char units)
{
	unsigned int tempDistance = 0;
	switch (sense) {
		case FRONT_SENSE:
			tempDistance = a2dConvert10bit(FRONT_SENSE);
			break;
		case SIDE_SENSE:
			tempDistance = a2dConvert10bit(SIDE_SENSE);
			break;
		default:
			break;
	}

	tempDistance = (5000/1023)*tempDistance;
	if(tempDistance > 3000)
	{
		return 10;
	}
	if(tempDistance < 350)
	{
		return 80;
	}

	for(int i = 0; i < (tableEndDistance - tableStartDistance - 1); i++)
	{
		double maxValue = pgm_read_word_near(distanceVoltageValue + i);
		double minValue = pgm_read_word_near(distanceVoltageValue + i + 1);
		double medValue = minValue + (maxValue - minValue)/2;
		if((tempDistance > minValue) &
		   (tempDistance <= medValue))
		{
			tempDistance = (i + tableStartDistance);
			break;
		}
		if((tempDistance > medValue) &
		   (tempDistance <= maxValue))
		{
			tempDistance = (i + tableStartDistance + 1);
			break;
		}
	}
	switch (units) {
		case MM_SENSE:
			return tempDistance * 10;
			break;
		case CM_SENSE:
			return tempDistance;
			break;
		default:
			break;
	}
	return -1;
}
