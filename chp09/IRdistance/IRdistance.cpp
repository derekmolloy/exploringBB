/* An IR distance sensor application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
using namespace std;

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

int readAnalog(int number){
   stringstream ss;
   ss << LDR_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

int main(int argc, char* argv[]){
   cout << "Starting the IR distance sensor program:" << endl;
   for(int i=0; i<1000; i++){
      int value = readAnalog(0);
      float distance = 115.804f * exp(-0.000843107f * (float)value);
      cout << "The distance is: " << distance << " cm" << '\r' << flush;
      usleep(100000);
   }
   return 0;
}
