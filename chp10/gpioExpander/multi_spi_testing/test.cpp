/**
 * testSPI.cpp This is a test program to utilize the MCP23S17 directly
 * using raw SPI calls according to the datasheet. It sets up BANKA and
 * BANKB as output pins and writes 10101010 and 01010101 to the bank
 * respectively. If LEDs are attached to PORT A (pins 21-28) and PORT B
 * (pins 8-1) then they will light in an alternating pattern.
 */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "gpioExpander.h"

using namespace std;
using namespace exploringRPi;

int main(){
   unsigned char receive; // used to store the send/rec data
   cout << "Starting the SPI GPIO Expander Example" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);
   spiDevice->setSpeed(488000);      // have access to SPI Device object
   spiDevice->setMode(SPIDevice::MODE0); // device supports mode 0 and 3
   GPIOExpander *gpio = new GPIOExpander(spiDevice,0x00);

   gpio->writeDevice(0x0A, 0b00111010); //set control regs.
   gpio->writeDevice(0x00, 0b00000000); //set port A as outputs
   gpio->writeDevice(0x01, 0b00000000); //set port B as outputs
   gpio->writeDevice(0x02, 0b00000000); //set regular polarity
   gpio->writeDevice(0x03, 0b00000000); //set regular polarity
   gpio->writeDevice(0x14, 0b10101010); //set output latch state
   gpio->writeDevice(0x15, 0b01010101); //set output latch state

   receive = gpio->readDevice(0x0A);
   cout << "At IOCON Received [" << (int)receive << "]" << endl;
   receive = gpio->readDevice(0x12);
   cout << "At GPIOA Received [" << (int)receive << "]" << endl;
   receive = gpio->readDevice(0x14);
   cout << "At OLATA Received [" << (int)receive << "]" << endl;

   cout << "End of the GPIO Expander Example" << endl;
}
