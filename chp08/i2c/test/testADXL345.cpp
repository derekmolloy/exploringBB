/** Simple I2C example to read the first address of a device in C++
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<iomanip>
using namespace std;

// Small macro to display value in hexadecimal with 2 places
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
#define DEVID       0x00
#define BUFFER_SIZE 40

int main(){
   int file;
   cout << "Starting the ADXL345 test application" << endl;
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      cout << "failed to open the bus" << endl;
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x53) < 0){
      cout << "Failed to connect to the sensor" << endl;
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(file, writeBuffer, 1)!=1){
      cout << "Failed to reset the read address" << endl;
      return 1;
   }
   char readBuffer[BUFFER_SIZE];
   if(read(file, readBuffer, BUFFER_SIZE)!=BUFFER_SIZE){
      cout << "Failed to read in the buffer" << endl;
      return 1;
   }
   cout << "The Device ID is: " << HEX(readBuffer[DEVID]) << endl;
   close(file);
   return 0;
}
