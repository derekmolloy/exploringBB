/** BB-BONE-GPIO Test code to test the GPIO-KEYS interface.
* Written by Derek Molloy (www.derekmolloy.ie) for the book
* Exploring BeagleBone.
*
* This code is based on work in the document:
*    www.kernel.org/doc/Documentation/input/input.txt
*/

#include<iostream>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/input.h>
#include"GPIO.h"
using namespace std;
using namespace exploringBB;

#define KEY_PRESS 1
#define KEY_RELEASE 0

int main(){
   int fd, count=0;
   struct input_event event[64];
   GPIO outGPIO(60);
   outGPIO.setDirection(OUTPUT);
   outGPIO.streamOpen();
   outGPIO.streamWrite(LOW);
   if(getuid()!=0){
      cout << "You must run this program as root. Exiting." << endl;
      return -1;
   }
   cout << "Starting BB-BONE-GPIO Test (press 10 times to end):" << endl;
   if ((fd = open("/dev/input/event1", O_RDONLY)) < 0){
      perror("Failed to open event1 input device. Exiting.");
      return -1;
   }
   while(count < 20){  // Press and Release are one loop each
      int numbytes = (int)read(fd, event, sizeof(event));
      if (numbytes < (int)sizeof(struct input_event)){
         perror("The input read was invalid. Exiting.");
         return -1;
      }
      for (int i=0; i < numbytes/sizeof(struct input_event); i++){
         int type = event[i].type;
         int val  = event[i].value;
         int code = event[i].code;
         if (type == EV_KEY) {
            if (val == KEY_PRESS){
               outGPIO.streamWrite(HIGH);
               cout << "Press  : Code "<< code <<" Value "<< val<< endl;
            }
            if (val == KEY_RELEASE){
               outGPIO.streamWrite(LOW);
               cout << "Release: Code "<< code <<" Value "<< val<< endl;
            }
         }
      }
      count++;
   }
   outGPIO.streamClose();
   close(fd);
   return 0;
}

