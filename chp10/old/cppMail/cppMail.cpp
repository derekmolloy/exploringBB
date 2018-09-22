/* A C++ e-mail application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

int main(){
   string to("toname@destination.com");
   string subject("Hello Derek");
   string body("Test Message body...");
   stringstream command;
   command << "echo \"" << body << "\" | mail -s \"" << subject << "\" " << to;
   int result = system(command.str().c_str());
   cout << "Command: " << command.str() << endl;
   cout << "The return value was " << result << endl;
   return result;
}
