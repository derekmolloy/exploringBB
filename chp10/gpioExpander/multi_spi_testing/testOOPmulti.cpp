/**
 * testOOPmulti.cpp A test program that outputs a different set of values
 * on PORTA of two different SPI gpioExpanders that are on the same bus.
 */

#include <iostream>
#include <sstream>
#include "gpioExpander.h"

using namespace std;
using namespace exploringRPi;

int main(){
   cout << "Starting the GPIO Multi Expander Example" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);
   spiDevice->setSpeed(488000);      // Have access to SPI Device object
   spiDevice->setMode(SPIDevice::MODE0);
   GPIOExpander gpio1(spiDevice, 0x00);
   GPIOExpander gpio2(spiDevice, 0x01);

   cout << "The GPIO Expander was set up successfully" << endl;
   gpio1.setGPIODirections(GPIOExpander::PORTA, 0b00000000); //output=0
   gpio2.setGPIODirections(GPIOExpander::PORTA, 0b00000000); //output=0
   gpio1.setOutputValues(GPIOExpander::PORTA, 0b11000011);
   gpio2.setOutputValues(GPIOExpander::PORTA, 0b10100101);

   cout << "The registers on the device at the 0x00 address are:" << endl;
   gpio1.dumpRegisters();
   cout << "The registers on the device at the 0x01 address are:" << endl;
   gpio2.dumpRegisters();
   cout << "End of the GPIO Multi SPI Expander Example" << endl;
}
