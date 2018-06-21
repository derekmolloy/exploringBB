/*
 * BusDevice.h  Created on: 23 May 2014
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

#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

namespace exploringBB {

/**
 * @class BusDevice
 * @brief This class is the parent of I2C and SPI devices, so that devices that use both
 * SPI and I2C interfaces can use those interfaces interchangeably. Because it contains
 * abstract methods, the child classes MUST implement the methods that are listed in this
 * class.
 */
class BusDevice {
protected:
	unsigned int bus;    /**< the bus number */
	unsigned int device; /**< the device number on the bus  */
	int file;            /**< the file handle to the device */
public:
	BusDevice(unsigned int bus, unsigned int device);
	virtual int open()=0;
	virtual unsigned char readRegister(unsigned int registerAddress)=0;
	virtual unsigned char* readRegisters(unsigned int number, unsigned int fromAddress=0)=0;
	virtual int write(unsigned char value)=0;
	virtual int writeRegister(unsigned int registerAddress, unsigned char value)=0;
	virtual void debugDumpRegisters(unsigned int number = 0xff)=0;
	virtual void close()=0;
	virtual ~BusDevice();
};

} /* namespace exploringBB */

#endif /* BUSDEVICE_H_ */
