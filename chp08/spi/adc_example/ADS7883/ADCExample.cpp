/* Using a 12-bit SPI ADC (e.g., the ADS7883)
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"

using namespace std;
using namespace exploringBB;

short combineValues(unsigned char upper, unsigned char lower){
   return ((short)upper<<6)|((short)lower>>2);
}

int main(){
   cout << "Starting EBB SPI ADC Example" << endl;
   SPIDevice *busDevice = new SPIDevice(1,0); //Using second SPI bus (both loaded)
   busDevice->setSpeed(4000000);      // Have access to SPI Device object
   busDevice->setMode(SPIDevice::MODE1);

   unsigned char send[2], receive[2];
   send[0] = 0; // No send data required for this IC
   send[1] = 0;
   busDevice->transfer(send, receive, 2);
   cout << "Response bytes are " << (int)receive[0] << "," << (int)receive[1] << endl;

   // Use the 8-bits of the second value and the two LSBs of the first value
   int value = combineValues(receive[0]&0b00111111, receive[1]);
   cout << "This is the value " << value << " out of 4096." << endl;
   cout << "End of EBB SPI ADC Example" << endl;
}
