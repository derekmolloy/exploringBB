#include <iostream>
#include "bus/SPIDevice.h"
using namespace exploringBB;
using namespace std;

int main(){
   cout << "Starting the MAX7219 example" << endl;
   SPIDevice *max = new SPIDevice(1,1);
   max->setSpeed(10000000);              // max speed is 10MHz
   max->setMode(SPIDevice::MODE0);

   // turn on the display and disable test mode -- just in case:
   max->writeRegister(0x0C, 0x01);
   max->writeRegister(0x0F, 0x00);
   max->writeRegister(0x0B, 0x07);       // set 8 digit mode
   max->writeRegister(0x09, 0xFF);       // set decode mode on all

   for(int i=1; i<9; i++){
      max->writeRegister((unsigned int)i, 0x0A); // clear to dashes
   }

   for(int i=0; i<=100000; i++){
      int val = i;
      unsigned int place = 1;
      while(val>0){
        max->writeRegister( place++, (unsigned char) val%10);
        val = val/10;
      }
//    usleep(1000);
   }
   max->close();
   cout << "End of the MAX7219 example" << endl;
   return 0;
}
