/*
 * LCDCharacterDisplay.cpp  Created on: 24 May 2014
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

#include "LCDCharacterDisplay.h"
#include <unistd.h>

// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY 		0b00000001
#define LCD_RETURN_HOME 		0b00000010
#define LCD_ENTRY_MODE_SET 		0b00000100
	#define ENTRY_MODE_LEFT		0b00000010
    #define ENTRY_MODE_S        0b00000001
#define LCD_DISPLAY_ON_OFF 		0b00001000
	#define DISPLAY_ENTIRE		0b00000100
	#define DISPLAY_CURSOR		0b00000010
	#define	DISPLAY_CURSOR_POS	0b00000001
#define LCD_CURSOR_DISPLAY   	0b00010000
    #define CURSOR_DISPLAY_SC   0b00001000
	#define CURSOR_DISPLAY_RL   0b00000100
#define LCD_FUNCTION_SET     	0b00100000
#define LCD_CGRAM_ADDR			0b01000000
#define LCD_DDRAM_ADDR			0b10000000

#define LCD_LONG_DELAY 			1520
#define LCD_SHORT_DELAY			37
#define LCD_ROW_OFFSET_ADDR		0x40

// Wiring Configuration for the 74HC595
// D7 D6 D5 D4 0  0  E  RS  (on the display module)
// H  G  F  E  D  C  B  A   (on the 74HC595)
// RS 1=data 0=command
// write set
// E operation enable signal (falling edge triggered)

namespace exploringBB {

/***
 * Constructor for the display. Needs an SPIDevice pointer and the number of characters
 * in the display, described by a width and height.
 * @param device SPIDevice bus and device ID pointer
 * @param width Number of columns in the display
 * @param height Number of rows in the display
 */
LCDCharacterDisplay::LCDCharacterDisplay(SPIDevice *device, int width, int height) {
	this->device  = device;
	this->width   = width;
	this->height  = height;

	//Default Cursor, Display and Entry states
	this->cursorState  = LCD_CURSOR_DISPLAY;
	this->displayState = LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE | DISPLAY_CURSOR | DISPLAY_CURSOR_POS;
	this->entryState   = LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT;

	this->setup4bit();
}

/***
 * Send a character command to the display module. These commands are generally set with
 * constants that are defined in the LCDCharacterDisplay.cpp file. The command is an 8-bit
 * value, but it is sent to the module in two parts - the lower 4-bits followed by the
 * upper 4-bits. This data is sent using the code as two 8-bit values, constructed as:
 *
 * 	D7 D6 D5 D4 0  0  E  RS
 *
 * where D7-D4 are the bits of the 8-bit values. 0, 0 are unused. The E bit needs to be
 * toggled high->low to write the value to the display and the RS states if the value being
 * sent is a command or data. For this method, which sends commands, RS will always be 0.
 *
 * @param i The character to send as a command
 */
void LCDCharacterDisplay::command(char i){

	char upper = (i << 4) & 0b11110000;
	char lower = i & 0b11110000;
	// need to write the lower data and toggle the E bit
	this->device->write(lower | 0b00000010);  //lower 4 bits
	usleep(1);				  //sleep for at least 300ns
	this->device->write(lower | 0b00000000);  //lower 4 bits
	// need to write the upper data and toggle the E bit
	this->device->write(upper | 0b00000010);  //lower 4 bits
	usleep(1);				  //sleep for at least 300ns
	this->device->write(upper | 0b00000000);  //lower 4 bits
}

/***
 * Initializes and sets up the display as described in the display data sheets.
 * This setup routine sets up a 4-bit display (sets 4-bit mode), sets the cursor,
 * turns on the display and sets entry mode.
 */
void LCDCharacterDisplay::setup4bit(){
	this->device->write(0x32);  //Manual write of Wake up!(first)
	usleep(LCD_LONG_DELAY);     //Sleep for at least 5ms
	this->device->write(0x30);  //Toggle the E bit, sends on falling edge
	usleep(LCD_SHORT_DELAY);    //Sleep for at least 160us
	this->device->write(0x32);  //Manual write of Wake up! (second)
	usleep(LCD_SHORT_DELAY);
	this->device->write(0x30);
	usleep(LCD_SHORT_DELAY);
	this->device->write(0x32);  //Manual write of Wake up! (third)
	usleep(LCD_SHORT_DELAY);
	this->device->write(0x30);
	usleep(LCD_SHORT_DELAY);
	this->device->write(0x22);  //Set 4-bit mode
	usleep(LCD_SHORT_DELAY);
	this->device->write(0x20);
	usleep(LCD_SHORT_DELAY);
	this->command(0x28);        //Set 4-bit/2-line
	// Default Cursor, Display and Entry states set in the constructor
	this->writeCursorState();   //Set cursor visible
	this->writeDisplayState();  //Display ON; Blinking cursor
	this->writeEntryState();
}

void LCDCharacterDisplay::writeCursorState(){
	this->command(this->cursorState);
	usleep(LCD_SHORT_DELAY);
}

void LCDCharacterDisplay::writeDisplayState(){
	this->command(this->displayState);
	//printf("Display[%02x]", this->displayState);
	usleep(LCD_SHORT_DELAY);
}

void LCDCharacterDisplay::writeEntryState(){
	this->command(this->entryState);
	usleep(LCD_SHORT_DELAY);
}

/***
 * Print a std::string to the character display at the current cursor position.
 * @param message
 */
void LCDCharacterDisplay::print(std::string message){
	for(unsigned int i=0; i<message.length(); i++){
		this->write(message[i]);
	}
}

/***
 * Send data to the display module. The data is an 8-bit value, but it is sent to the module
 * in two parts - the lower 4-bits followed by the upper 4-bits. This data is sent using the
 * code as two 8-bit values, constructed as:
 *
 * 	D7 D6 D5 D4 0  0  E  RS
 *
 * where D7-D4 are the bits of the 8-bit values. 0, 0 are unused. The E bit needs to be
 * toggled high->low to write the data to the display and the RS states if the value being
 * sent is a command or data. For this method, which sends data, RS will always be 1.
 *
 * @param i The character to send as data to the display module
 */
void LCDCharacterDisplay::write(char c){
	// 4-bit mode. Send lower 4 bits followed by higher 4 bits
	char upper = (c << 4) & 0b11110000;
	char lower = c & 0b11110000;
	// need to write the lower data and toggle the E bit
	this->device->write(lower | 0b00000011);  //lower 4 bits
	usleep(1);					  //sleep for at least 300ns
	this->device->write(lower | 0b00000001);  //lower 4 bits
	// need to write the upper data and toggle the E bit
	this->device->write(upper | 0b00000011);  //lower 4 bits
	usleep(1);					  //sleep for at least 300ns
	this->device->write(upper | 0b00000001);  //lower 4 bits
}

/***
 * Clears the display by passing the LCD_CLEAR_DISPLAY command
 */
void LCDCharacterDisplay::clear(){
	this->command(LCD_CLEAR_DISPLAY);
	usleep(LCD_LONG_DELAY);  //data sheets states that a delay of 1.52ms is required
}

/***
 * Returns the cursor to the 00 address by passing the LCD_RETURN_HOME command
 */
void LCDCharacterDisplay::home(){
	this->command(LCD_RETURN_HOME);
	usleep(LCD_LONG_DELAY);  //data sheets states that a delay of 1.52ms is required
}

/***
 * Sets the Cursor position on the display module. 0,0 is the top-left corner. Address
 * 0x00 is row 0, 0x40 is row 1. So, (1,5) would be the second row and the 6th column
 * over. Display addresses are in the order:
 *   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
 *   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
 * For a 2x16 character display
 * @param row The number of rows in the display (e.g., 2)
 * @param column The number of characters wide the display is (e.g., 16)
 * @return -1 if the row or column is greater than the number of rows or columns
 */
int LCDCharacterDisplay::setCursorPosition(int row, int column){

	if ((column>=this->width)||(row>=this->height)) return -1;
    row = row * LCD_ROW_OFFSET_ADDR;
    int value = (row + column) | LCD_DDRAM_ADDR;
	this->command(value);
	//printf("[%02x]", value);
    return 0;
}

/**
 * Turn the display on or off.
 * @param displayOff pass true to turn the display off, false to turn it back on
 */
void LCDCharacterDisplay::setDisplayOff(bool displayOff){
	if (displayOff){
		this->displayState = displayState & (~DISPLAY_ENTIRE); //bit inversion of DISPLAY_ENTIRE
		this->writeDisplayState();
	}
	else{
		this->displayState = displayState | DISPLAY_ENTIRE;
		this->writeDisplayState();
	}
}

/**
 * Turn the cursor on or off.
 * @param cursorOff pass true to turn the cursor off, false to turn it back on
 */
void LCDCharacterDisplay::setCursorOff(bool cursorOff){
	if (cursorOff){
		this->displayState = displayState & (~DISPLAY_CURSOR);
		this->writeDisplayState();
	}
	else{
		this->displayState = displayState | DISPLAY_CURSOR;
		this->writeDisplayState();
	}
}

/**
 * Turn the blink on or off.
 * @param isBlink pass true to turn the blink on, false to turn it off
 */
void LCDCharacterDisplay::setCursorBlink(bool isBlink){
	if (!isBlink){
		this->displayState = displayState & (~DISPLAY_CURSOR_POS); //bit inversion of DISPLAY_ENTIRE
		this->writeDisplayState();
	}
	else{
		this->displayState = displayState | DISPLAY_CURSOR_POS;
		this->writeDisplayState();
	}
}

/**
 * Turn the cursor moving On or Off
 * @param cursorOff
 */
void LCDCharacterDisplay::setCursorMoveOff(bool cursorMoveOff){
	if (!cursorMoveOff){
		this->cursorState = cursorState & (~CURSOR_DISPLAY_SC);
		this->writeCursorState();
	}
	else{
		this->cursorState = cursorState | CURSOR_DISPLAY_SC;
		this->writeCursorState();
	}
}

/**
 *
 * @param cursorMoveLeft
 */
void LCDCharacterDisplay::setCursorMoveLeft(bool cursorMoveLeft){
	if (!cursorMoveLeft){
		this->cursorState = cursorState & (~CURSOR_DISPLAY_RL);
		this->writeCursorState();
	}
	else{
		this->cursorState = cursorState | CURSOR_DISPLAY_RL;
		this->writeCursorState();
	}
}

/**
 *
 * @param setAutoscroll
 */
void LCDCharacterDisplay::setAutoscroll(bool isAutoscroll){
	if (!isAutoscroll){
		this->entryState = entryState & (~ENTRY_MODE_S);
		this->writeEntryState();
	}
	else{
		this->entryState = entryState | ENTRY_MODE_S;
		this->writeEntryState();
	}
}

/**
 *
 * @param scrollDisplayLeft
 */
void LCDCharacterDisplay::setScrollDisplayLeft(bool scrollLeft){
	if (scrollLeft){
		this->entryState = entryState & (~ENTRY_MODE_LEFT);
		this->writeEntryState();
	}
	else{
		this->entryState = entryState | ENTRY_MODE_LEFT;
		this->writeEntryState();
	}
}


/***
 * Destructor - closes the SPI device file handle.
 */
LCDCharacterDisplay::~LCDCharacterDisplay() {
	this->device->close();
}

} /* namespace exploringBB */
