/** Simple /proc/uptime parsing program
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(int argc, char* argv[]){
   cout << "Starting the read uptime program" << endl;
   std::fstream fs;
   fs.open("/proc/uptime", std::fstream::in);
   float uptime, idletime;
   fs >> uptime >> idletime;
   cout << "The system up time is " << uptime/60 << " minutes.\n";
   cout << "It was idle for " << idletime/60 << " minutes, or "
        <<  100*(idletime/uptime) << "%\n";
   fs.close();
   return 0;
}

