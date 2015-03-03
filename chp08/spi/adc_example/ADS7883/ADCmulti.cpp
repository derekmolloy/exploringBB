/* Using an SPI ADC (e.g., the MCP3008)
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"
#define SAMPLES 2000

using namespace std;
using namespace exploringBB;

short combineValues(unsigned char upper, unsigned char lower){
   return ((short)upper<<6)|((short)lower>>2);
}

int main(){
//   cout << "Starting EBB SPI ADC Multi Example" << endl;
   SPIDevice *busDevice = new SPIDevice(1,0); //Using second SPI bus (both loaded)
   busDevice->setSpeed(4000000);      // Have access to SPI Device object
   busDevice->setMode(SPIDevice::MODE0);

   unsigned char send[2], receive[2];
   int samples[SAMPLES];

   send[0] = 0; // No data to send
   send[1] = 0;
   for(int i=0; i<SAMPLES; i++){
   	busDevice->transfer(send, receive, 2);
        samples[i]=combineValues(receive[0]&0b00111111, receive[1]);
   }
//   cout << "The samples that were captured are:" << endl;
   for(int i=0; i<SAMPLES; i++){
        cout << i << " " << samples[i] << endl;
   }
//   cout << "\nEnd of EBB SPI ADC Example" << endl;
}
