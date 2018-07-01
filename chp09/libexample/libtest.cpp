#include <iostream>
#include <sstream>
#include "display/LCDCharacterDisplay.h"
using namespace exploringBB;
using namespace std;

int main() {
   cout << "Testing the EBB library" << endl;
   SPIDevice *busDevice = new SPIDevice(1,1);
   busDevice->setSpeed(1000000);      // access to SPI Device object
   ostringstream s;                   // using to combine text and ints
   LCDCharacterDisplay display(busDevice, 20, 2); // a 20x4 display
   display.clear();                   // Clear the character LCD module
   display.home();                    // Move to the (0,0) position
   display.print("   Exploring RBB");
   cout << "End of the EBB library test" << endl;
   return 0;
}
