/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#ifndef SENSORSAMPLE_H
#define SENSORSAMPLE_H

#include <QMetaType>
/**
 * @brief The SensorSample class provides a simple data transfer object that can be used
 * to send a sample using a signal to a slot. The signal is generated when the XML parser
 * identifies an object and the slot is attached to the main window -- it is used to
 * update the GUI components.
 */
class SensorSample
{
    float pitch;
    float roll;
public:
    SensorSample();   // The macro at the bottom requires a default constructor
    SensorSample(float pitch, float roll);

    float getPitch() { return pitch; }     //simple getter methods
    float getRoll() { return roll; }
};

// The macro below is very important as it allows the SensorSample class to be set up
// as a type that can be transferred with signals. This step is usually the only step
// required; however, in this program the signals are being sent between different
// threads and there is a second step. See the main.cpp file, where there is a call to
// the line:
//    qRegisterMetaType<SensorSample>();
// which is require in Qt to pass a custom object as a signal between threads.

Q_DECLARE_METATYPE(SensorSample)

#endif // SENSORSAMPLE_H
