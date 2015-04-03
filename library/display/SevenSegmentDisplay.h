/*
 * SevenSegmentDisplay.h  Created on: 6 Jun 2014
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

#ifndef SEVENSEGMENTDISPLAY_H_
#define SEVENSEGMENTDISPLAY_H_
#include "../bus/SPIDevice.h"

namespace exploringBB {

/**
 * @class SevenSegmentDisplay
 * @brief A class that allows you to drive an array of 7 segment displays using an array of 74XX595 ICs
 */
class SevenSegmentDisplay {
private:
	SPIDevice *spidevice; //!< The SPI bus device
	int numberSegments;   //!< The number of segments connected
	int numberBase;       //!< Base can be between 2 (binary) and 16 (hexadecimal). Default is decimal (base 10)
	bool isCommonAnode;   //!< Is a common anode display -- false by default

public:
	SevenSegmentDisplay(SPIDevice *device, int numberSegments);
	virtual int write(int number);                // writes an integer
	virtual int write(float number, int places);  // uses the "decimal" point
	virtual int setNumberBase(int base);
	virtual int getNumberBase() { return this->numberBase; }
	virtual int getNumberSegments() { return this->numberSegments; }
	virtual void setCommonAnode(bool isCommonAnode) { this->isCommonAnode = isCommonAnode; }
	virtual ~SevenSegmentDisplay();       // closes the SPI device

private:
	const static unsigned char symbols[]; //!< The array of symbols (i.e., characters to LED segments)
};

} /* namespace exploringBB */

#endif /* SEVENSEGMENTDISPLAY_H_ */
