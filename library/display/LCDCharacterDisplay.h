/*
 * LCDCharacterDisplay.h  Created on: 24 May 2014
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

#ifndef LCDCHARACTERDISPLAY_H_
#define LCDCHARACTERDISPLAY_H_
#include "../bus/SPIDevice.h"
#include <string>

namespace exploringBB {

/**
 * @class LCDCharacterDisplay
 * @brief A class that provides an interface to an LCD character module. It provices support
 * for multiple rows and columns and provides methods for formatting and printing text. You
 * should use a 4 wire interface and a 74XX595 to communicate with the display module.
 */

class LCDCharacterDisplay {

private:
	SPIDevice *device;          //!< a pointer to the SPI device
	int width, height;          //!< the width and height of the module in characters
	void command(char i);
	void setup4bit();
	unsigned char cursorState;
	unsigned char displayState;
	unsigned char entryState;
	void writeCursorState();
	void writeDisplayState();
	void writeEntryState();

public:
	LCDCharacterDisplay(SPIDevice *device, int width, int height);

	virtual void write(char c);
	virtual void print(std::string message);

	virtual void clear();
	virtual void home();
	virtual int  setCursorPosition(int row, int column);
	virtual void setDisplayOff(bool displayOff);
	virtual void setCursorOff(bool cursorOff);
	virtual void setCursorBlink(bool isBlink);
	virtual void setCursorMoveOff(bool cursorMoveOff);
	virtual void setCursorMoveLeft(bool cursorMoveLeft);
	virtual void setAutoscroll(bool isAutoscroll);
	virtual void setScrollDisplayLeft(bool scrollLeft);

	virtual ~LCDCharacterDisplay();
};

} /* namespace exploringBB */

#endif /* LCDCHARACTERDISPLAY_H_ */
