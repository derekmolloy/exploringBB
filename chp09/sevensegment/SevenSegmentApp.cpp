/* A Seven-segment display application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include "display/SevenSegmentDisplay.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB Seven Segment Display Example" << endl;
   SevenSegmentDisplay display(new SPIDevice(1,0), 2);
   //counting in hexadecimal
   //display.setCommonAnode(true); //For a common anode display setup
   cout << "Counting in hexadecimal 0x00 to 0xFF" << endl;
   display.setNumberBase(16); //count in hexadecimal
   for(int i=0; i<=0xFF; i++){
      display.write(i);
      usleep(50000);
   }
   cout << "Counting 0.0 to 9.9 decimal in steps of 0.1..." << endl;
   display.setNumberBase(10); //count in decimal
   for(float f=0.0f; f<10; f+=0.1f){
      display.write(f,1);
      usleep(100000);
   }
   cout << "End of EBB Seven Segment Display Example" << endl;
}
