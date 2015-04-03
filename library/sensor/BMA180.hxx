/*
 * BMA180.h  Created on: 2 Jul 2014
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

#ifndef BMA180_H_
#define BMA180_H_
#include "../bus/BusDevice.h"
#define BUFFER_SIZE 0x80

namespace exploringBB {

/**
 * @class BMA180
 * @brief A class to control a BMA180 accelerometer (untested)
 */
class BMA180 {

	enum RANGE {
		PLUSMINUS_1_G 		= 0,
		PLUSMINUS_1POINT5_G = 1,
		PLUSMINUS_2G 		= 2,
		PLUSMINUS_3G 		= 3,
		PLUSMINUS_4G 		= 4,
		PLUSMINUS_8G 		= 5,
		PLUSMINUS_16G 		= 6
	};

	enum BANDWIDTH {
		BW_10HZ 	= 0,
		BW_20HZ 	= 1,
		BW_40HZ 	= 2,
		BW_75HZ 	= 3,
		BW_150HZ 	= 4,
		BW_300HZ 	= 5,
		BW_600HZ 	= 6,
		BW_12OOHZ 	= 7,
		BW_HIGHPASS = 8,
		BW_BANDPASS = 9
	};

	enum MODECONFIG {
		MODE_LOW_NOISE = 0,
		MODE_LOW_POWER = 3
	};


private:
	BusDevice *device;
	unsigned char *registers;

	int accelerationX;
	int accelerationY;
	int accelerationZ;

	double pitch;  //in degrees
	double roll;   //in degrees

	float temperature; //accurate to 0.5C
	BMA180::RANGE range;
	BMA180::BANDWIDTH bandwidth;
	BMA180::MODECONFIG modeConfig;

	int  convertAcceleration(int msb_addr, int lsb_addr);
	void calculatePitchAndRoll();

public:
	BMA180(BusDevice *device);
	void displayMode(int iterations);

	int  readFullSensorState();
	// The following do physical reads and writes of the sensors
	int setRange(BMA180::RANGE range);
	BMA180::RANGE getRange();
	int setBandwidth(BMA180::BANDWIDTH bandwidth);
	BMA180::BANDWIDTH getBandwidth();
	int setModeConfig(BMA180::MODECONFIG mode);
	BMA180::MODECONFIG getModeConfig();
	float getTemperature();

	int getAccelerationX() { return accelerationX; }
	int getAccelerationY() { return accelerationY; }
	int getAccelerationZ() { return accelerationZ; }

	float getPitch() { return pitch; }  // in degrees
	float getRoll() { return roll; }  // in degrees

	virtual ~BMA180();
};

} /* namespace exploringBB */

#endif /* BMA180_H_ */
