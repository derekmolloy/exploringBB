/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "sensorsample.h"

SensorSample::SensorSample(){
    this->pitch = 0.0f;
    this->roll = 0.0f;
}

SensorSample::SensorSample(float pitch, float roll)
{
    this->pitch = pitch;
    this->roll = roll;
}
