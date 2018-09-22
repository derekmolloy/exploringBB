/*
 * SPIDevice.h  Created on: 22 May 2015
 * Copyright (c) 2015 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 */

#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_
#include<string>
#include<stdint.h>
#include"BusDevice.h"

#define SPI_PATH "/dev/spidev"

namespace exploringRPi{

/**
 * @class SPIDevice
 * @brief Generic SPI Device class that can be used to connect to any type of SPI device and
 * read or write to its registers
 */
class SPIDevice:public BusDevice {
public:
	/// The SPI Mode
	enum SPIMODE{
		MODE0 = 0,   //!< Low at idle, capture on rising clock edge
		MODE1 = 1,   //!< Low at idle, capture on falling clock edge
		MODE2 = 2,   //!< High at idle, capture on falling clock edge
		MODE3 = 3    //!< High at idle, capture on rising clock edge
	};

private:
	std::string filename;

public:
	SPIDevice(unsigned int bus, unsigned int device);
	virtual int open();
	virtual unsigned char readRegister(unsigned int registerAddress);
	virtual unsigned char* readRegisters(unsigned int number, unsigned int fromAddress=0);
	virtual int writeRegister(unsigned int registerAddress, unsigned char value);
	virtual void debugDumpRegisters(unsigned int number = 0xff);
	virtual int write(unsigned char value);
	virtual int write(unsigned char value[], int length);
	virtual int setSpeed(uint32_t speed);
	virtual int setMode(SPIDevice::SPIMODE mode);
	virtual int setBitsPerWord(uint8_t bits);
	virtual void close();
	virtual ~SPIDevice();
	virtual int transfer(unsigned char read[], unsigned char write[], int length);

private:
	SPIMODE mode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
};

} /* namespace exploringRPi */

#endif /* SPIDEVICE_H_ */
