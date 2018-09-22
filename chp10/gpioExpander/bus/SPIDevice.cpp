/*
 * SPIDevice.cpp  Created on: 22 June 2015
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 */

#include "SPIDevice.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
using namespace std;

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

namespace exploringRPi {

/**
 * The constructor for the SPIDevice that sets up and opens the SPI connection.
 * The destructor will close the SPI file connection.
 * @param bus The SPI bus number X (first digit after spidevX.Y)
 * @param device The device on the bus Y (second digit after spidevX.Y)
 */
SPIDevice::SPIDevice(unsigned int bus, unsigned int device):
	BusDevice(bus,device) {
	stringstream s;
	s << SPI_PATH << bus << "." << device;
	this->filename = string(s.str());
	this->mode = SPIDevice::MODE3;
	this->bits = 8;
	this->speed = 488000;
	this->delay = 0;
	this->open();
}

/**
 * This method opens the file connection to the SPI device.
 * @return 0 on a successful open of the file
 */
int SPIDevice::open(){
	//cout << "Opening the file: " << filename.c_str() << endl;
	if ((this->file = ::open(filename.c_str(), O_RDWR))<0){
		perror("SPI: Can't open device.");
		return -1;
	}
	if (this->setMode(this->mode)==-1) return -1;
	if (this->setSpeed(this->speed)==-1) return -1;
	if (this->setBitsPerWord(this->bits)==-1) return -1;
	return 0;
}

/**
 * Generic method to transfer data to and from the SPI device. It is used by the
 * following methods to read and write registers.
 * @param send The array of data to send to the SPI device
 * @param receive The array of data to receive from the SPI device
 * @param length The length of the array to send
 * @return -1 on failure
 */
int SPIDevice::transfer(unsigned char send[], unsigned char receive[], int length){
	struct spi_ioc_transfer	transfer;
	transfer.tx_buf = (unsigned long) send;
	transfer.rx_buf = (unsigned long) receive;
	transfer.len = length;
	transfer.speed_hz = this->speed;
	transfer.delay_usecs = this->delay;
	transfer.bits_per_word = this->bits;
        transfer.cs_change = 0;
	transfer.tx_nbits = 0;
	transfer.rx_nbits = 0;
	transfer.pad = 0;
	int status = ioctl(this->file, SPI_IOC_MESSAGE(1), &transfer);
	if (status < 0) {
		perror("SPI: Transfer SPI_IOC_MESSAGE Failed");
		return -1;
	}
	return status;
}

unsigned char SPIDevice::readRegister(unsigned int registerAddress){
	unsigned char send[2], receive[2];
	memset(send, 0, sizeof send);
	memset(receive, 0, sizeof receive);
	send[0] = (unsigned char) (0x80 + registerAddress);
	this->transfer(send, receive, 2);
        cout << "The value that was received is: " << (int) receive[1] << endl;
	return receive[1];
}

unsigned char* SPIDevice::readRegisters(unsigned int number, unsigned int fromAddress){
	unsigned char* data = new unsigned char[number];
	unsigned char send[number+1], receive[number+1];
	memset(send, 0, sizeof send);
	send[0] =  (unsigned char) (0x80 + 0x40 + fromAddress); //set read bit and MB bit
	this->transfer(send, receive, number+1);
	memcpy(data, receive+1, number);  //ignore the first (address) byte in the array returned
	return data;
}

int SPIDevice::write(unsigned char value){
	unsigned char null_return = 0x00;
	printf("[%02x]", value);
	this->transfer(&value, &null_return, 1);
	return 0;
}

int SPIDevice::write(unsigned char value[], int length){
	unsigned char null_return = 0x00;
	this->transfer(value, &null_return, length);
	return 0;
}

int SPIDevice::writeRegister(unsigned int registerAddress, unsigned char value){
	unsigned char send[2], receive[2];
	memset(receive, 0, sizeof receive);
	send[0] = (unsigned char) registerAddress;
	send[1] = value;
	cout << "The value that was written is: " << (int) send[1] << endl;
	this->transfer(send, receive, 2);
	return 0;
}

void SPIDevice::debugDumpRegisters(unsigned int number){
	cout << "SPI Mode: " << this->mode << endl;
	cout << "Bits per word: " << (int)this->bits << endl;
	cout << "Max speed: " << this->speed << endl;
	cout << "Dumping Registers for Debug Purposes:" << endl;
	unsigned char *registers = this->readRegisters(number);
	for(int i=0; i<(int)number; i++){
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}

int SPIDevice::setSpeed(uint32_t speed){
	this->speed = speed;
	if (ioctl(this->file, SPI_IOC_WR_MAX_SPEED_HZ, &this->speed)==-1){
		perror("SPI: Can't set max speed HZ");
		return -1;
	}
	if (ioctl(this->file, SPI_IOC_RD_MAX_SPEED_HZ, &this->speed)==-1){
		perror("SPI: Can't get max speed HZ.");
		return -1;
	}
	return 0;
}

int SPIDevice::setMode(SPIDevice::SPIMODE mode){
	this->mode = mode;
	if (ioctl(this->file, SPI_IOC_WR_MODE, &this->mode)==-1){
		perror("SPI: Can't set SPI mode.");
		return -1;
	}
	if (ioctl(this->file, SPI_IOC_RD_MODE, &this->mode)==-1){
		perror("SPI: Can't get SPI mode.");
		return -1;
	}
	return 0;
}

int SPIDevice::setBitsPerWord(uint8_t bits){
	this->bits = bits;
	if (ioctl(this->file, SPI_IOC_WR_BITS_PER_WORD, &this->bits)==-1){
		perror("SPI: Can't set bits per word.");
		return -1;
	}
	if (ioctl(this->file, SPI_IOC_RD_BITS_PER_WORD, &this->bits)==-1){
		perror("SPI: Can't get bits per word.");
		return -1;
	}
	return 0;
}

void SPIDevice::close(){
	::close(this->file);
	this->file = -1;
}

SPIDevice::~SPIDevice() {
	this->close();
}

} /* namespace exploringRPi */
