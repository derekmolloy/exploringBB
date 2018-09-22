/*
 * I2C.cpp  Created on: 17 May 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
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

#include"I2CDevice.h"
#include<iostream>
#include<sstream>
#include<fcntl.h>
#include<iomanip>
#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
using namespace std;

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

namespace exploringRPi {

/**
 * Constructor for the I2CDevice class. It requires the bus number and device number. The constructor
 * opens a file handle to the I2C device, which is destroyed when the destructor is called
 * @param bus The bus number. Usually 0 or 1 on the RPi
 * @param device The device ID on the bus.
 */
I2CDevice::I2CDevice(unsigned int bus, unsigned int device):
	BusDevice(bus,device){
	this->open();
}

/**
 * Open a connection to an I2C device
 * @return 1 on failure to open to the bus or device, 0 on success.
 */
int I2CDevice::open(){
   string name;
   if(this->bus==0) name = RPI_I2C_0;
   else name = RPI_I2C_1;

   if((this->file=::open(name.c_str(), O_RDWR)) < 0){
      perror("I2C: failed to open the bus\n");
	  return 1;
   }
   if(ioctl(this->file, I2C_SLAVE, this->device) < 0){
      perror("I2C: Failed to connect to the device\n");
	  return 1;
   }
   return 0;
}

/**
 * Write a single byte value to a single register.
 * @param registerAddress The register address
 * @param value The value to be written to the register
 * @return 1 on failure to write, 0 on success.
 */

int I2CDevice::writeRegister(unsigned int registerAddress, unsigned char value){
   unsigned char buffer[2];
   buffer[0] = registerAddress;
   buffer[1] = value;
   if(::write(this->file, buffer, 2)!=2){
      perror("I2C: Failed write to the device\n");
      return 1;
   }
   return 0;
}

/**
 * Write a single value to the I2C device. Used to set up the device to read from a
 * particular address.
 * @param value the value to write to the device
 * @return 1 on failure to write, 0 on success.
 */
int I2CDevice::write(unsigned char value){
   unsigned char buffer[1];
   buffer[0]=value;
   if (::write(this->file, buffer, 1)!=1){
      perror("I2C: Failed to write to the device\n");
      return 1;
   }
   return 0;
}

/**
 * Read a single register value from the address on the device.
 * @param registerAddress the address to read from
 * @return the byte value at the register address.
 */
unsigned char I2CDevice::readRegister(unsigned int registerAddress){
   this->write(registerAddress);
   unsigned char buffer[1];
   if(::read(this->file, buffer, 1)!=1){
      perror("I2C: Failed to read in the value.\n");
      return 1;
   }
   return buffer[0];
}

/**
 * Method to read a number of registers from a single device. This is much more efficient than
 * reading the registers individually. The from address is the starting address to read from, which
 * defaults to 0x00.
 * @param number the number of registers to read from the device
 * @param fromAddress the starting address to read from
 * @return a pointer of type unsigned char* that points to the first element in the block of registers
 */
unsigned char* I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress){
	this->write(fromAddress);
	unsigned char* data = new unsigned char[number];
    if(::read(this->file, data, number)!=(int)number){
       perror("IC2: Failed to read in the full buffer.\n");
	   return NULL;
    }
	return data;
}

/**
 * Method to dump the registers to the standard output. It inserts a return character after every
 * 16 values and displays the results in hexadecimal to give a standard output using the HEX() macro
 * that is defined at the top of this file. The standard output will stay in hexadecimal format, hence
 * the call on the last like.
 * @param number the total number of registers to dump, defaults to 0xff
 */

void I2CDevice::debugDumpRegisters(unsigned int number){
	cout << "Dumping Registers for Debug Purposes:" << endl;
	unsigned char *registers = this->readRegisters(number);
	for(int i=0; i<(int)number; i++){
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}

/**
 * Close the file handles and sets a temporary state to -1.
 */
void I2CDevice::close(){
	::close(this->file);
	this->file = -1;
}

/**
 * Closes the file on destruction, provided that it has not already been closed.
 */
I2CDevice::~I2CDevice() {
	if(file!=-1) this->close();
}

} /* namespace exploringRPi */
