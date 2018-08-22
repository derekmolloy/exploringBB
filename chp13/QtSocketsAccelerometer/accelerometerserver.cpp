/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "accelerometerserver.h"

AccelerometerServer::AccelerometerServer(int port, MainWindow *parent):
    QTcpServer(parent)
{
    this->portNumber = port;
    this->parent = parent;
}

void AccelerometerServer::incomingConnection(int socketDescriptor)
{
    this->thread = new ServerThread(socketDescriptor, parent, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
