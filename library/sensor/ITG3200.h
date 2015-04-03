/*
 * ITG3200Gyroscope.h  Created on: 2 Jul 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef ITG3200GYROSCOPE_H_
#define ITG3200GYROSCOPE_H_

#include "../bus/BusDevice.h"

#define BUFFER_SIZE       0x3E
#define SENSITIVITY	      14.375  //LSBs per degree

namespace exploringBB {

/**
 * @class ITG3200
 * @brief A class to interface with the ITG3200 gyroscope (untested)
 */
class ITG3200 {

	// This is required for the DLPF_CFG value - Digital low-pass filter configuration
	enum ITG3200_LPFILTER {
		LP_256HZ 	= 0,    // sets internal sampling rate Fs to be 8kHz
		LP_188HZ 	= 1,   	// sets internal sampling rate Fs to be 1kHz
		LP_98HZ 	= 2,	// sets internal sampling rate Fs to be 1kHz
		LP_42HZ 	= 3,	// sets internal sampling rate Fs to be 1kHz
		LP_20HZ 	= 4,	// sets internal sampling rate Fs to be 1kHz
		LP_10HZ 	= 5,	// sets internal sampling rate Fs to be 1kHz
		LP_5HZ  	= 6	// sets internal sampling rate Fs to be 1kHz
					// 7 is a reserved value
	};

private:
	BusDevice *device;
	unsigned char *registers;
	float gyroscope[3];
	float offsets[3];
	int temperature;

public:

	ITG3200(BusDevice *device);

	float getGyroscopeRoll() { return gyroscope[0]/SENSITIVITY; }
	float getGyroscopePitch() { return gyroscope[1]/SENSITIVITY; }
	float getGyroscopeYaw() { return gyroscope[2]/SENSITIVITY; }

	float getOffsetRollOffset() { return offsets[0]/SENSITIVITY; }
	float getOffsetPitchOffset() { return offsets[1]/SENSITIVITY; }
	float getOffsetYawOffset() { return offsets[2]/SENSITIVITY; }

	int getTemperature() { return temperature; }

	virtual ~ITG3200();

public:
	int readFullSensorState();
	int convertGyroscopeValue(int msb_reg_addr, int lsb_reg_addr);
	int setDigitalLowPassFilter(ITG3200_LPFILTER filterValue);
	int setSampleRateDivider(char divider);
	int zeroCalibrate(int numberSamples, int sampleDelayMs);
};

} /* namespace exploringBB */

#endif /* ITG3200GYROSCOPE_H_ */
