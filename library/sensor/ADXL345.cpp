/*
 * ADXL345.cpp  Created on: 17 May 2014
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

#include "ADXL345.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringBB {

//From Table 19. of the ADXL345 Data sheet
#define DEVID          0x00   //Device ID
#define THRESH_TAP     0x1D   //Tap Threshold
#define OFSX           0x1E   //X-axis Offset
#define OFSY           0x1F   //Y-axis Offset
#define OFSZ           0x20   //Z-axis Offset
#define DUR            0x21   //Tap duration
#define LATENT         0x22   //Tap latency
#define WINDOW         0x23   //Tap window
#define THRESH_ACT     0x24   //Activity threshold
#define THRESH_INACT   0x25   //Threshold inactivity
#define TIME_INACT     0x26   //Inactivity time
#define ACT_INACT_CTL  0x27   //Axis enable control for activity and inactivity detection
#define THRESH_FF      0x28   //Free-fall threshold
#define TIME_FF        0x29   //Free-fall time
#define TAP_AXES       0x2A   //Axis control for single tap/double tap
#define ACT_TAP_STATUS 0x2B   //Source of single tap/double tap
#define BW_RATE        0x2C   //Data rate and power mode control
#define POWER_CTL      0x2D   //Power-saving features control
#define INT_ENABLE     0x2E   //Interrupt enable control
#define INT_MAP        0x2F   //Interrupt mapping control
#define INT_SOURCE     0x30   //Source of interrupts
#define DATA_FORMAT    0x31   //Data format control
#define DATAX0         0x32   //X-axis Data 0
#define DATAX1         0x33   //X-axis Data 1
#define DATAY0         0x34   //Y-axis Data 0
#define DATAY1         0x35   //Y-axis Data 1
#define DATAZ0         0x36   //Z-axis Data 0
#define DATAZ1         0x37   //Z-axis Data 1
#define FIFO_CTL       0x38   //FIFO control
#define FIFO_STATUS    0x39   //FIFO status

/**
 * Method to combine two 8-bit registers into a single short, which is 16-bits on the BBB. It shifts
 * the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
short ADXL345::combineRegisters(unsigned char msb, unsigned char lsb){
   //shift the MSB right by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

/**
 * Method to calculate the pitch and roll state values. This calculation takes account of the scaling
 * factors due to the resolution and gravity range to determine gravity weighted values that are used
 * to calculate the angular pitch and roll values in degrees.
 */
void ADXL345::calculatePitchAndRoll(){
	float gravity_range;
	switch(ADXL345::range){
		case ADXL345::PLUSMINUS_16_G: gravity_range=32.0f; break;
		case ADXL345::PLUSMINUS_8_G: gravity_range=16.0f; break;
		case ADXL345::PLUSMINUS_4_G: gravity_range=8.0f; break;
		default: gravity_range=4.0f; break;
	}
    float resolution = 1024.0f;
    if (this->resolution==ADXL345::HIGH) resolution = 8192.0f; //13-bit resolution
    float factor = gravity_range/resolution;

    float accXg = this->accelerationX * factor;
    float accYg = this->accelerationY * factor;
    float accZg = this->accelerationZ * factor;
	float accXSquared = accXg * accXg ;
	float accYSquared = accYg * accYg ;
	float accZSquared = accZg * accZg ;
	this->pitch = 180 * atan(accXg/sqrt(accYSquared + accZSquared))/M_PI;
	this->roll = 180 * atan(accYg/sqrt(accXSquared + accZSquared))/M_PI;
}

/**
 * Method used to update the DATA_FORMAT register and any other registers that might be added
 * in the future.
 * @return 0 if the register is updated successfully
 */
int ADXL345::updateRegisters(){
   //update the DATA_FORMAT register
   char data_format = 0x00;  //+/- 2g with normal resolution
   //Full_resolution is the 3rd LSB
   data_format = data_format|((this->resolution)<<3);
   data_format = data_format|this->range; // 1st and 2nd LSB therefore no shift
   return this->device->writeRegister(DATA_FORMAT, data_format);
}

/**
 * The constructor for the ADXL345 accelerometer object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the ADXL345 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */

ADXL345::ADXL345(BusDevice *busDevice){
	//this->I2CAddress = I2CAddress;
	//this->I2CBus = I2CBus;
	this->device = busDevice;
	this->accelerationX = 0;
	this->accelerationY = 0;
	this->accelerationZ = 0;
	this->pitch = 0.0f;
	this->roll = 0.0f;
	this->registers = NULL;
	this->range = ADXL345::PLUSMINUS_16_G;
	this->resolution = ADXL345::HIGH;
	this->device->writeRegister(POWER_CTL, 0x08);
	this->updateRegisters();
}

/**
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the ADXL345 sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
int ADXL345::readSensorState(){
	this->registers = this->device->readRegisters(BUFFER_SIZE, 0x00);
	if(*this->registers!=0xe5){
		perror("ADXL345: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->accelerationX = this->combineRegisters(*(registers+DATAX1), *(registers+DATAX0));
	this->accelerationY = this->combineRegisters(*(registers+DATAY1), *(registers+DATAY0));
	this->accelerationZ = this->combineRegisters(*(registers+DATAZ1), *(registers+DATAZ0));
	this->resolution = (ADXL345::RESOLUTION) (((*(registers+DATA_FORMAT))&0x08)>>3);
	this->range = (ADXL345::RANGE) ((*(registers+DATA_FORMAT))&0x03);
	this->calculatePitchAndRoll();
	return 0;
}

/**
 * Set the ADXL345 gravity range according to the RANGE enumeration
 * @param range One of the four possible gravity ranges defined by the RANGE enumeration
 */
void ADXL345::setRange(ADXL345::RANGE range) {
	this->range = range;
	updateRegisters();
}

/**
 * Set the ADXL345 resolution according to the RESOLUTION enumeration
 * @param resolution either HIGH or NORMAL resolution. HIGH resolution is only available if the range is set to +/- 16g
 */
void ADXL345::setResolution(ADXL345::RESOLUTION resolution) {
	this->resolution = resolution;
	updateRegisters();
}

/**
 * Useful debug method to display the pitch and roll values in degrees on a single standard output line
 * @param iterations The number of 0.1s iterations to take place.
 */
void ADXL345::displayPitchAndRoll(int iterations){
	int count = 0;
	while(count < iterations){
	      cout << "Pitch:"<< this->getPitch() << " Roll:" << this->getRoll() << "     \r"<<flush;
	      usleep(100000);
	      this->readSensorState();
	      count++;
	}
}

ADXL345::~ADXL345() {}

} /* namespace exploringBB */
