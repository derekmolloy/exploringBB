/*
 * BusDevice.h  Created on: 23 May 2015
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 */

#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

namespace exploringRPi {

/**
 * @class BusDevice
 * @brief This class is the parent of I2C and SPI devices, so that devices that use both
 * SPI and I2C interfaces can use those interfaces interchangeably. Because it contains
 * abstract methods, the child classes MUST implement the methods that are listed in this
 * class. This means that the
 */
class BusDevice {
protected:
	unsigned int bus;
	unsigned int device;
	int file;
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

} /* namespace exploringRPi */

#endif /* BUSDEVICE_H_ */
