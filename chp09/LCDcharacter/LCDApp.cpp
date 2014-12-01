/* A LCD Character Display applications
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include "display/LCDCharacterDisplay.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB LCD Character Display Example" << endl;
   SPIDevice *busDevice = new SPIDevice(2,0); //Using second SPI bus (both loaded)
   busDevice->setSpeed(1000000);      // Have access to SPI Device object
   ostringstream s;                   // Using this to combine text and int data
   LCDCharacterDisplay display(busDevice, 16, 2); // Construct 16x2 LCD Display
   display.clear();                   // Clear the character LCD module
   display.home();                    // Move the cursor to the (0,0) position
   display.print("EBB by D. Molloy"); // String to display on the first row
   for(int x=0; x<=10000; x++){       // Do this 10,000 times
      s.str("");                      // clear the ostringstream object s
      display.setCursorPosition(1,3); // move the cursor to second row
      s << "X=" << x;                 // construct a string that has an int value
      display.print(s.str());         // print the string X=*** on the LCD module
   }
   cout << "End of EBB LCD Character Display Example" << endl;
}
