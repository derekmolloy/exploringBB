/* A LCD Character Display applications
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"
#include "sensor/ADXL345.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB ADXL345 SPI Test" << endl;
   SPIDevice *busDevice = new SPIDevice(1,0); // Using second SPI bus (both loaded)
   busDevice->setSpeed(1000000);              // Have access to SPI Device object
   ADXL345 acc(busDevice);
   acc.displayPitchAndRoll(100);
   cout << "End of EBB ADXL345 SPI Test" << endl;
}
