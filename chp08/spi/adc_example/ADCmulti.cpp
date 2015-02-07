/* Using an SPI ADC (e.g., the MCP3008)
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"
#define SAMPLES 200

using namespace std;
using namespace exploringBB;

short combineValues(unsigned char upper, unsigned char lower){
   return ((short)upper<<8)|(short)lower;
}

int main(){
//   cout << "Starting EBB SPI ADC Multi Example" << endl;
   SPIDevice *busDevice = new SPIDevice(1,0); //Using second SPI bus (both loaded)
   busDevice->setSpeed(4000000);      // Have access to SPI Device object
   busDevice->setMode(SPIDevice::MODE0);

   unsigned char send[3], receive[3];
   int samples[SAMPLES];

   send[0] = 0b00000001; // The Start Bit followed
   // Set the SGL/Diff and D mode -- e.g., 1000 means single ended CH0 value
   send[1] = 0b10000000; // The MSB is the Single/Diff bit and it is followed by 000 for CH0
   send[2] = 0;          // This byte doesn't need to be set, just for a clear display

   for(int i=0; i<SAMPLES; i++){
   	busDevice->transfer(send, receive, 3);
        samples[i]=combineValues(receive[1]&0b00000011, receive[2]);
   }
//   cout << "The samples that were captured are:" << endl;
   for(int i=0; i<SAMPLES; i++){
        cout << i << " " << samples[i] << endl;
   }
//   cout << "\nEnd of EBB SPI ADC Example" << endl;
}
