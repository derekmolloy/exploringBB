/** Simple On-board LED flashing program - written by Derek Molloy
*    simple functional struture for the Exploring BeagleBone book
*    changed to be a multi-call binary
*
*    This program uses USR LED 3 and can be executed in three ways:
*         makeLED on
*         makeLED off
*         makeLED flash  (flash at 100ms intervals - on 50ms/off 50ms)
*         makeLED status (get the trigger status)
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define LED3_PATH "/sys/class/leds/beaglebone:green:usr3"

void writeLED(string filename, string value){
   fstream fs;
   string path(LED3_PATH);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

void removeTrigger(){
  writeLED("/trigger", "none");
}

bool endsWith(string const &in, string const &comp){
   return (0 == in.compare(in.length()-comp.length(), comp.length(), comp));
}

int main(int argc, char* argv[]){
   if(argc!=2){
	cout << "Usage is makeLEDmulti is by calling one of: " << endl;
        cout << "   flashled, onled, offled, ledstatus" << endl;
	cout << " e.g. sudo ./flashled" << endl;
   }
   string cmd(argv[0]);
   cout << "Starting the makeLEDmulti program" << endl;
   cout << "The current LED Path is: " << LED3_PATH << endl;

   // select whether command is on, off, flash or status
   if(endsWith(cmd,"onled")){
        cout << "Turning the LED on" << endl;
	removeTrigger();
        writeLED("/brightness", "1");
   }
   else if (endsWith(cmd,"offled")){
        cout << "Turning the LED off" << endl;
	removeTrigger();
        writeLED("/brightness", "0");
   }
   else if (endsWith(cmd,"flashled")){
        cout << "Flashing the LED" << endl;
        writeLED("/trigger", "timer");
        writeLED("/delay_on", "50");
        writeLED("/delay_off", "50");
   }
   else if (endsWith(cmd,"ledstatus")){
	// display the current trigger details
        std::fstream fs;
	fs.open( LED3_PATH "/trigger", std::fstream::in);
	string line;
	while(getline(fs,line)) cout << line << endl;
	fs.close();
   }
   else{
	cout << "Invalid command!" << endl;
   }
   cout << "Finished the makeLEDmulti Program" << endl;
   return 0;
}
