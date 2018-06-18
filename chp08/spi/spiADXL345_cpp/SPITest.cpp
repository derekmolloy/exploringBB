#include <iostream>
#include <sstream>
#include "bus/SPIDevice.h"
#include "sensor/ADXL345.h"
using namespace std;
using namespace exploringBB;

int main(){
   SPIDevice spi(0,0);
   spi.setSpeed(5000000);
   cout << "The device ID is: " << (int) spi.readRegister(0x00) << endl;
   spi.setMode(SPIDevice::MODE3);
   spi.writeRegister(0x2D, 0x08);
   spi.debugDumpRegisters(0x40);
}
