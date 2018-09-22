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
   unsigned char send[3], receive[3]; // used to store the send/rec data
   cout << "Starting the SPI GPIO Expander Example" << endl;
   SPIDevice *spiDevice = new SPIDevice(0,0);
   spiDevice->setSpeed(488000);      // have access to SPI Device object
   spiDevice->setMode(SPIDevice::MODE0);

   spiDevice->open();  // Open for the following comands -- close afterwards
   send[0]=0b01000000; // writing to address 000 (r/w bit = 0)
   send[1]=0x0A;       // IOCON -- configuration and control register
   send[2]=0b00111010; // set up the MCP23S17 (IOCON.BANK=0)
   cout << "Writing " << (int)send[2] << " to addr " << (int)send[1] << endl;
   spiDevice->transfer(send,receive,3);

   send[0]=0b01000000; // writing to address 000 (r/w bit = 0)
   send[1]=0x00;       // IODIRA -- Input/Output Direction Register PORTA
   send[2]=0b00000000; // set all of PORTA as outputs (0=output,1=input)
   cout << "Writing " << (int)send[2] << " to addr " << (int)send[1] << endl;
   spiDevice->transfer(send,receive,3);

   send[0]=0b01000000; // writing to address 000 (r/w bit = 0)
   send[1]=0x14;       // OLATA -- Output latch settings
   send[2]=0b01010101; // set each alternate output on/off
   cout << "Writing " << (int)send[2] << " to addr " << (int)send[1] << endl;
   spiDevice->transfer(send,receive,3);

   send[0]=0b01000001; // reading from 000 (r/w bit = 1)
   send[1]=0x0A;       // check that the IOCON was set as above
   send[2]=0x00;       // not necessary (cleans up output)
   spiDevice->transfer(send,receive,3);
   cout << "At IOCON Received [" << (int) receive[2] << "]" << endl;

   send[0]=0b01000001; // reading from 000 (r/w bit = 1)
   send[1]=0x12;       // GPIOA reading the GPIO values of PORTA
   spiDevice->transfer(send,receive,3);
   cout << "At GPIOA Received [" << (int) receive[2] << "]" << endl;

   send[0]=0b01000001; // reading from 000 (r/w bit = 1)
   send[1]=0x14;       // OLATA reading the output latches on PORTA
   spiDevice->transfer(send,receive,3);
   cout << "At OLATA Received [" << (int) receive[2] << "]" << endl;

   spiDevice->close();
   cout << "End of the GPIO Expander Example" << endl;
}
