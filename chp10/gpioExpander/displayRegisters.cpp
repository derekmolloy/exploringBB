/**
 * testSPI.cpp This is a test program to utilize the MCP23S17 directly
 * using raw SPI calls according to the datasheet. It sets up BANKA as
 * output pins and writes 01010101 to the bank. If LEDs are attached
 * to PORT A (pins 21-28) then the will light alternately.
 */

#include <iostream>
#include <sstream>
#include "gpioExpander.h"

using namespace std;
using namespace exploringRPi;

int main(){
   unsigned char receive; // used to store the send/rec data
   cout << "Starting the SPI GPIO Expander Example" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);
   spiDevice->setSpeed(488000);      // have access to SPI Device object
   spiDevice->setMode(SPIDevice::MODE3); // device supports mode 0 and 3
   GPIOExpander *gpio = new GPIOExpander(spiDevice,0x00);
   gpio->dumpRegisters();
   cout << "End of the GPIO Expander Example" << endl;
}
