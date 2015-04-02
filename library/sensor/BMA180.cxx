/*
 * BMA180.cpp  Created on: 2 Jul 2014
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

#include "BMA180.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#define ACC_X_LSB 	    0x02
#define ACC_X_MSB 	    0x03
#define ACC_Y_LSB 	    0x04
#define ACC_Y_MSB 	    0x05
#define ACC_Z_LSB 	    0x06
#define ACC_Z_MSB 	    0x07
#define BMA_TEMP	  	0x08  //Temperature
#define BMA_RANGE	  	0x35  //bits 3,2,1
#define BMA_BANDWIDTH 	0x20  //bits 7,6,5,4
#define MODE_CONFIG     0x30  //bits 1,0

namespace exploringBB {

BMA180::BMA180(BusDevice *device) {
	this->device = device;
	readFullSensorState();
}

void BMA180::calculatePitchAndRoll() {
	double accelerationXSquared = this->accelerationX * this->accelerationX;
	double accelerationYSquared = this->accelerationY * this->accelerationY;
	double accelerationZSquared = this->accelerationZ * this->accelerationZ;
	this->pitch = 180 * atan(accelerationX/sqrt(accelerationYSquared + accelerationZSquared))/M_PI;
	this->roll = 180 * atan(accelerationY/sqrt(accelerationXSquared + accelerationZSquared))/M_PI;
}

int BMA180::readFullSensorState(){
	this->registers = this->device->readRegisters(BUFFER_SIZE, 0x00);
	if(*this->registers!=0x03){
		perror("BMA180: Failure Condition - Sensor ID not Verified");
		return -1;
	}
    this->accelerationX = convertAcceleration(ACC_X_MSB, ACC_X_LSB);
    this->accelerationY = convertAcceleration(ACC_Y_MSB, ACC_Y_LSB);
    this->accelerationZ = convertAcceleration(ACC_Z_MSB, ACC_Z_LSB);
    this->calculatePitchAndRoll();
    //cout << "Pitch:" << this->getPitch() << "   Roll:" << this->getRoll() <<  endl;
    return 0;
}

int BMA180::convertAcceleration(int msb_reg_addr, int lsb_reg_addr){
//	cout << "Converting " << (int) dataBuffer[msb_reg_addr] << " and " << (int) dataBuffer[lsb_reg_addr] << endl;;
	short temp = *(registers+msb_reg_addr);
	temp = (temp<<8) | *(registers+lsb_reg_addr);
	temp = temp>>2;
	temp = ~temp + 1;
//	cout << "The X acceleration is " << temp << endl;
	return temp;
}

void BMA180::displayMode(int iterations){
	for(int i=0; i<iterations; i++){
		this->readFullSensorState();
		printf("Rotation (%d, %d, %d)", accelerationX, accelerationY, accelerationZ);
	}
}

//  Temperature in 2's complement has a resolution of 0.5K/LSB
//  80h is lowest temp - approx -40C and 00000010 is 25C in 2's complement
//  this value is offset at room temperature - 25C and accurate to 0.5K

float BMA180::getTemperature(){

	int offset = -40;  // -40 degrees C
	this->readFullSensorState();
	char temp = *(registers+BMA_TEMP);   // = -80C 0b10000000  0b00000010; = +25C
	//char temp = this->readI2CDeviceByte(TEMP);
	//this->readFullSensorState();
    //char temp = dataBuffer[TEMP];
	int temperature;
	if(temp&0x80)	{
		temp = ~temp + 0b00000001;
		temperature = 128 - temp;
	}
	else {
		temperature = 128 + temp;
	}
	this->temperature = offset + ((float)temperature*0.5f);
	//cout << "The temperature is " << this->temperature << endl;
	//int temp_off = dataBuffer[0x37]>>1;
	//cout << "Temperature offset raw value is: " << temp_off << endl;
	return this->temperature;
}

BMA180::RANGE BMA180::getRange(){
	this->readFullSensorState();
	char temp = *(registers+BMA_RANGE);
	//char temp = this->readI2CDeviceByte(RANGE);  //bits 3,2,1
	temp = temp & 0b00001110;
	temp = temp>>1;
	//cout << "The current range is: " << (int)temp << endl;
	this->range = (BMA180::RANGE) temp;
	return this->range;
}

int BMA180::setRange(BMA180::RANGE range){
	//char current = this->readI2CDeviceByte(RANGE);  //bits 3,2,1
	this->readFullSensorState();
	char current = *(registers+BMA_RANGE);
	char temp = range << 1; //move value into bits 3,2,1
	current = current & 0b11110001; //clear the current bits 3,2,1
	temp = current | temp;
	if(this->device->writeRegister(BMA_RANGE,temp)!=0){
		perror("Failure to update RANGE value\n");
		return 1;
	}
	return 0;
}

BMA180::BANDWIDTH BMA180::getBandwidth(){
	this->readFullSensorState();
	char temp = *(registers+BMA_BANDWIDTH);   //bits 7->4
	//char temp = this->readI2CDeviceByte(BANDWIDTH);  //bits 7,6,5,4
//	cout << "The value of bandwidth returned is: " << (int)temp << endl;
	temp = temp & 0b11110000;
	temp = temp>>4;
//	cout << "The current bandwidth is: " << (int)temp << endl;
	this->bandwidth = (BMA180::BANDWIDTH) temp;
	return this->bandwidth;
}

int BMA180::setBandwidth(BMA180::BANDWIDTH bandwidth){
	this->readFullSensorState();
    char current = *(registers+BMA_BANDWIDTH);  //bits 7->4
	char temp = bandwidth << 4; //move value into bits 7,6,5,4
	current = current & 0b00001111; //clear the current bits 7,6,5,4
	temp = current | temp;
	if(this->device->writeRegister(BMA_BANDWIDTH,temp)!=0){
		perror("Failure to update BANDWIDTH value\n");
		return 1;
	}
	return 0;
}

BMA180::MODECONFIG BMA180::getModeConfig(){
	//char temp = dataBuffer[MODE_CONFIG];   //bits 1,0
	//char temp = this->readI2CDeviceByte(MODE_CONFIG);  //bits 1,0
	this->readFullSensorState();
    char temp = *(registers+MODE_CONFIG);
	temp = temp & 0b00000011;
	this->modeConfig = (BMA180::MODECONFIG) temp;
	return this->modeConfig;
}

} /* namespace exploringBB */
