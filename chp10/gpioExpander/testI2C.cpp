/**
 * testI2C.cpp This is a test program to utilize the MCP23017 directly
 * using raw I2C calls according to the datasheet. It sets up BANKA as
 * output pins and writes 01010101 to the bank. If LEDs are attached
 * to PORT A (pins 21-28) then the will light alternately.
 */

#include <iostream>
#include <sstream>
#include "gpioExpander.h"

using namespace std;
using namespace exploringRPi;

int main(){
   unsigned char value;
   cout << "Starting the SPI GPIO Expander Example" << endl;
   I2CDevice *i2cDevice = new I2CDevice(1,0x20);

   i2cDevice->open();  // Open for the following comands -- close afterwards
   i2cDevice->writeRegister(0x05, 0b10111010); //set up the MCP23017
   cout << "Set up the Control Register - IOCON (0x05)" << endl;

   i2cDevice->writeRegister(0x00, 0b00000000); //set all of PORTA as outputs
   cout << "Set up PORTA as outputs using - IODIRA (0x00)" << endl;

   i2cDevice->writeRegister(0x0A, 0b01010101); //set alternate outputs on/off
   cout << "Set outputs on PORT A with alternate on/off outputs" << endl;

   value = i2cDevice->readRegister(0x05); // check that IOCON was set above
   cout << "At IOCON Received [" << (int)value << "]" << endl;

   value = i2cDevice->readRegister(0x09); // check that IOCON was set above
   cout << "At GPIOA Received [" << (int)value << "]" << endl;

   value = i2cDevice->readRegister(0x0A); // check that IOCON was set above
   cout << "At OLATA Received [" << (int)value << "]" << endl;

   i2cDevice->close();
   cout << "End of the GPIO Expander Example" << endl;
}
