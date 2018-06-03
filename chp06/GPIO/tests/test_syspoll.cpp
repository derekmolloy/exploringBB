#include<iostream>
#include<unistd.h>
#include"GPIO.h"
using namespace exploringBB;
using namespace std;

int main(){
   GPIO outGPIO(60), inGPIO(46);
   inGPIO.setDirection(INPUT);    //button is an input
   outGPIO.setDirection(OUTPUT);  //LED is an output
   inGPIO.setEdgeType(FALLING);   //wait for falling edge
   outGPIO.streamOpen();          //fast write, ready file
   outGPIO.streamWrite(LOW);      //turn the LED off
   cout << "Press the button:" << endl;
   inGPIO.waitForEdge();          //will wait forever
   outGPIO.streamWrite(HIGH);     //button pressed, light LED
   outGPIO.streamClose();         //close the output stream
   return 0;
}
