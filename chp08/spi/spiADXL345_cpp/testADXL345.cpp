#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"
#include "sensor/ADXL345.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB ADXL345 SPI Test" << endl;
   SPIDevice *busDevice = new SPIDevice(0,0); // Using second SPI bus (both loaded)
   busDevice->setSpeed(5000000);              // Have access to SPI Device object
   ADXL345 acc(busDevice);
   acc.displayPitchAndRoll(100);
   cout << "End of EBB ADXL345 SPI Test" << endl;
}
