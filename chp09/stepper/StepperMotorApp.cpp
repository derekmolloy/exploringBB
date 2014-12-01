/* A Stepper Motor Application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.
* The pins for this example are wired as follows:
* MS1 - BBB P8_12 = GPIO 1_12 = GPIO 44
* MS2 - BBB P8_18 = GPIO 2_01 = GPIO 65
* STEP- BBB P8_14 = GPIO 0_26 = GPIO 26
* SLP - BBB P8_11 = GPIO 1_13 = GPIO 45
* DIR - BBB P8_16 = GPIO 1_14 = GPIO 46
*/

#include <iostream>
#include "motor/StepperMotor.h"
using namespace std;
using namespace exploringBB;

int main(){
   cout << "Starting EBB Stepper Motor Example:" << endl;
   //Using 5 GPIOs, RPM=60 and 200 steps per revolution
   StepperMotor m(44,65,26,45,46,60,200);
   m.setDirection(StepperMotor::ANTICLOCKWISE);
   m.setStepMode(StepperMotor::STEP_FULL);
   m.setSpeed(100);  //rpm
   cout << "Rotating 10 times at 100 rpm anti-clockwise, full step..." << endl;
   m.rotate(3600.0f);   //in degrees
   cout << "Finished regular (non-threaded) rotation)" << endl;
   m.setDirection(StepperMotor::CLOCKWISE);
   cout << "Performing 1 threaded revolution in 5 seconds using micro-stepping:" << endl;
   m.setStepMode(StepperMotor::STEP_EIGHT);
   if(m.threadedStepForDuration(1600, 5000)<0){
      cout << "Failed to start the Stepper Thread" << endl;
   }
   cout << "Thread should now be running..." << endl;
      for(int i=0; i<10; i++){ // sleep for 10 seconds.
      usleep(1000000);
      cout << i+1 << " seconds has passed..." << endl;
   }
   m.sleep();   // cut power to the stepper motor
   cout << "End of Stepper Motor Example" << endl;
}
