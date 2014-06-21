/*
 * line_sense.c
 *
 *  Created on: Feb 25, 2014
 *      Author: proman
 */

#include "line_sense.h"

double lineSensVoltage[16];
unsigned char line[16];

const signed char mapLine[16] =
{
	-7, -6, -5, -4, -3, -2, -1, 0, 0, 1, 2, 3, 4, 5, 6, 7
};
//const signed char mapLine[16] =
//{
//	-4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 3, 3, 4
//};


const double mapVoltageLine[16] =
{
	1.2, 0.9, 0.3, 0.3, 0.3, 0.7, 0.7, 1.0, 1.0, 1.0, 0.3, 0.3, 0.3, 0.5, 0.9, 1.2
};

inline void selectCHIP1(void)
{
	sbi(PORT_CHIP2, PIN_CHIP2);
	cbi(PORT_CHIP1, PIN_CHIP1);
}

inline void selectCHIP2(void)
{
	sbi(PORT_CHIP1, PIN_CHIP1);
	cbi(PORT_CHIP2, PIN_CHIP2);
}

inline void stopAllCHIPS(void)
{
	sbi(PORT_CHIP1, PIN_CHIP1);
	sbi(PORT_CHIP2, PIN_CHIP2);
}

void lineSenseInit(void)
{
	// Initialization of chip selection pins
	sbi(PORT_CHIP1, PIN_CHIP1);
	sbi(PORT_CHIP2, PIN_CHIP2);
	sbi(DDR_CHIP1, PIN_CHIP1);
	sbi(DDR_CHIP2, PIN_CHIP2);

	spiInit();
}

double* lineSenseReadVoltage(void)
{
	unsigned char addrCH;
	uint8_t tempHighTx = 0, tempLowTx = 0;
	uint8_t voltageRetrunHigh, voltageRetrunLow;
	unsigned char addrVoltage = 0;

	for(addrCH = 0; addrCH < 8; addrCH++)
	{
		// Create data for sending to MCP3208
		tempLowTx = (addrCH & 0x03) << 6;
		tempHighTx = (0x06|((addrCH & 0x04) >> 2));

		// Sending and receiving data
		selectCHIP1();
		spiTransferByte(tempHighTx);
		voltageRetrunHigh = spiTransferByte(tempLowTx);
		voltageRetrunLow = spiTransferByte(0xFF);
		stopAllCHIPS();

		// Store the data
		lineSensVoltage[addrVoltage++] = (uint16_t)(((voltageRetrunHigh & 0x0F)<<8)|voltageRetrunLow);

		// Wait Disable time
		_delay_us(0.5);
	}

	for(addrCH = 0; addrCH < 8; addrCH++)
	{
		// Create data for sending to MCP3208
		tempLowTx = (addrCH & 0x03) << 6;
		tempHighTx = (0x06|((addrCH & 0x04) >> 2));

		// Sending and receiving data
		selectCHIP2();
		spiTransferByte(tempHighTx);
		voltageRetrunHigh = spiTransferByte(tempLowTx);
		voltageRetrunLow = spiTransferByte(0xFF);
		stopAllCHIPS();

		// Store the data
		lineSensVoltage[addrVoltage++] = (uint16_t)(((voltageRetrunHigh & 0x0F)<<8)|voltageRetrunLow);

		// Wait Disable time
		_delay_us(0.5);
	}

	for(addrVoltage = 0; addrVoltage < 16; addrVoltage++)
	{
		lineSensVoltage[addrVoltage] = V_REF * lineSensVoltage[addrVoltage] / 4096;
	}
	return lineSensVoltage;
}

unsigned char * lineSenseReadLine(void)
{
	lineSenseReadVoltage();

	for(int i = 0; i < 16; i++) {
		if(lineSensVoltage[i] > mapVoltageLine[i]) {
			line[i] = 1;
		} else {
			line[i] = 0;
		}
	}
	return line;
}

signed char lineSenseGetError(void)
{
	lineSenseReadLine();

	signed char error = 0;

	for(int i = 15; i > 8; i--) {
		if(line[i] == 1) {
			error = mapLine[i];
			return error;
		}
	}
	for(int i = 0; i < 7; i++) {
		if(line[i] == 1) {
			error = mapLine[i];
			return error;
		}
	}
	if(!line[7] && !line[8])
	{
		return -20;
	}

	return error;
}
