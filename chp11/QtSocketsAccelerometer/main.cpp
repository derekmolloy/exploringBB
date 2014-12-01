/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "mainwindow.h"
#include <QApplication>
#include "sensorsample.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<SensorSample>();   // needed to pass a custom object as a signal between threads
    MainWindow w;
    w.show();
    return a.exec();
}
