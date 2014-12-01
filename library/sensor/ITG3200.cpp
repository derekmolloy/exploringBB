/*
 * ITG3200Gyroscope.cpp  Created on: 2 Jul 2014
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

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include "ITG3200.h"
#include <iostream>
using namespace std;


#define WHOAMI 		0x00  // R/W  bits 6-1 (not bits 0 or 7)
#define GYRO_X_MSB 	0x1D  // R   7-0
#define GYRO_X_LSB 	0x1E  // R   7-0
#define GYRO_Y_MSB 	0x1F  // R   7-0
#define GYRO_Y_LSB 	0x20  // R   7-0
#define GYRO_Z_MSB 	0x21  // R   7-0
#define GYRO_Z_LSB 	0x22  // R   7-0
#define TEMP_MSB 	0x1B  // R   7-0
#define TEMP_LSB 	0x1C  // R   7-0
#define INT_CFG		0x17  // R/W 7 ACTL, 6 OPEN, 5 LATCH_INT_EN, 4 INT_ANYRD_2CLEAR, 2 ITG_RDY_EN, 0 RAW_RDY_EN
#define INT_STATUS	0x1A  // R   bit 2 ITG_RDY  bit 0 RAW_DATA_RDY
#define PWR_MGM		0x3E  // R/W bits 2-0 CLK_SEL, bit 7 H_RESET, 6 SLEEP, 5 STBY_XG, 4 STBY_YG, 3 STBY_ZG
#define SMPLRT_DIV	0x15  // R/W bits 7-0 sample rate divider (default 00h) divider = 0-255
						  // sample rate = Fint / (divider+1), where Fint = 1KHz or 8kHz
#define DLPF_FS		0x16  // R/W bits 4-3 FS_SEL bits 2-0 DLPF_CFG Full Scale
						  // power on reset value of FS_SEL (3/4) is 00h -> set to 03h for proper operation i.e. +/- 2000 deg/sec
						  // DLPF_CFG is the low pass filter sampling rate configuration
#define MAX_BUS 64

namespace exploringBB {

ITG3200::ITG3200(BusDevice *device) {
	this->device = device;
	for (int i=0; i<3; i++) {
		this->gyroscope[i] = 0.0;
		this->offsets[i] = 0.0;
	}
	this->registers = NULL;
	this->temperature = 0;
	this->readFullSensorState();
}

int ITG3200::readFullSensorState(){
	this->registers = this->device->readRegisters(BUFFER_SIZE, 0x00);
	if(*this->registers!=0x69){
		perror("ITG3200: Failure Condition - Sensor ID not Verified");
		return -1;
	}
    this->gyroscope[0] = convertGyroscopeValue(GYRO_X_MSB, GYRO_X_LSB);
    this->gyroscope[1] = convertGyroscopeValue(GYRO_Y_MSB, GYRO_Y_LSB);
    this->gyroscope[2] = convertGyroscopeValue(GYRO_Z_MSB, GYRO_Z_LSB);
    this->temperature = convertGyroscopeValue(TEMP_MSB, TEMP_LSB);
    return 0;
}

int ITG3200::convertGyroscopeValue(int msb_reg_addr, int lsb_reg_addr){
	short temp = *(registers+msb_reg_addr);
	temp = (temp<<8) | *(registers+lsb_reg_addr);
	temp = ~temp + 1;
	return temp;
}

int ITG3200::setDigitalLowPassFilter(ITG3200_LPFILTER filterValue){
	//bits 7,6,5 are 0;  bits 4,3 are = 03h for proper operation, i.e. +/- 2000 deg/sec full range
	char temp = 0b00011000;
	temp = temp | filterValue; // OR with incoming value (between 00 and 06h)
	if(this->device->writeRegister(DLPF_FS, temp)!=0){
		cout << "Failure to update ITG3200 DLPF_FS value" << endl;
		return 1;
	}
	return 0;
}

int ITG3200::zeroCalibrate(int numberSamples, int sampleDelayMs){

	double total[3] = {0.0, 0.0, 0.0};
	for(int i=0; i<numberSamples; i++){
		this->readFullSensorState();
		for(int j=0; j<3; j++){
			total[j] = total[j] + this->gyroscope[j];
		}
		usleep(sampleDelayMs*1000);
	}
	for(int i=0; i<=2; i++) {
		this->offsets[i] = total[i]/numberSamples;
	}
	return 0;
}

int ITG3200::setSampleRateDivider(char divider){
	//bits 7-0 set the divider which sets the sample rate of the gyro according to the equation
	// Fsample = Finternal / (divider+1)
	// Finternal is set according to the low pass filter ITG3200_LPFILTER
	// 	  which is 1kHz for all values except the highest LPF of DLPF_CFG
	if(this->device->writeRegister(SMPLRT_DIV, divider)!=0){
		cout << "Failure to update ITG3200 SMPLRT_DIV value" << endl;
		return 1;
	}
	return 0;
}

ITG3200::~ITG3200() {}

} /* namespace exploringBB */
