/*
 * SevenSegmentDisplay.cpp  Created on: 6 Jun 2014
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

#include "SevenSegmentDisplay.h"

namespace exploringBB {

// The binary data that describes the LED state for each symbol
// A(top) B(top right) C(bottom right) D(bottom)
// E(bottom left) F(top left) G(middle)  H(dot)
const unsigned char SevenSegmentDisplay::symbols[16] = { //(msb) HGFEDCBA (lsb)
     0b00111111, 0b00000110, 0b01011011, 0b01001111,     //0123
     0b01100110, 0b01101101, 0b01111101, 0b00000111,     //4567
     0b01111111, 0b01100111, 0b01110111, 0b01111100,     //89Ab
     0b00111001, 0b01011110, 0b01111001, 0b01110001      //CdEF
};

/**
 * The constructor for the 7-segment display that defines the number of segments.
 * @param device The pointer to the SPI device bus
 * @param numberSegments The number of 7-segment modules attached to the bus
 */
SevenSegmentDisplay::SevenSegmentDisplay(SPIDevice *device, int numberSegments) {
	this->spidevice = device;
	this->numberSegments = numberSegments;
	this->numberBase = 10; //decimal by default
	this->isCommonAnode = false;
}

int SevenSegmentDisplay::setNumberBase(int base){
	if (base>16 || base<2) return -1;
	return (this->numberBase = base);
}

int SevenSegmentDisplay::write(int number){
    // going to send one character for each segment
	unsigned char output[this->numberSegments];
	// output least-significant digit and divide by base
    for(int i=0; i<this->numberSegments; i++){
       output[i] = this->symbols[number%this->numberBase];
       if(this->isCommonAnode) output[i]=~output[i]; //invert the bits for common anode
       number = number/this->numberBase;
    }
    this->spidevice->write(output, this->numberSegments);
	return 0;
}

int SevenSegmentDisplay::write(float number, int places){
	// if the number of places is greater than the number of segments -1, stop.
	if (places>(this->numberSegments-1)) return -1;
	// can display non-decimal floats
	int intNumber = (int) number;
	if (places>0) intNumber = (int)(number * places * this->numberBase);
    // going to send one character for each segment
	unsigned char output[this->numberSegments];
	// output least-significant digit and divide by base
    for(int i=0; i<this->numberSegments; i++){
       output[i] = this->symbols[intNumber%this->numberBase];
       if(i==places) output[i] = output[i] | 0b10000000; // turn on "decimal point"
       if(this->isCommonAnode) output[i]=~output[i]; //invert the bits for common anode
       intNumber = intNumber/this->numberBase;
    }
    this->spidevice->write(output, this->numberSegments);
	return 0;
}

SevenSegmentDisplay::~SevenSegmentDisplay() {
	this->spidevice->close();
}

} /* namespace exploringBB */
