/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#ifndef ACCELEROMETERSERVER_H
#define ACCELEROMETERSERVER_H

#include <QTcpServer>
#include "mainwindow.h"
#include "serverthread.h"

class ServerThread;
class MainWindow;

/**
 * @brief The AccelerometerServer class is a helper class that is a child of the QTcpServer
 * class. It is used to create threads when there is an incoming connection. This is
 * somewhat unnecessary in this particular example, but it could be extended to provide
 * additional functionality to manage the server.
 */
class AccelerometerServer : public QTcpServer
{
    Q_OBJECT

public:
    /**
     * @brief AccelerometerServer The constructor to create the Server object
     * @param port The port number of the server, 4444 in this case
     * @param parent A reference back to the main user interface. This can be passed on to
     * the thread objects that the incomingConnection(); method creates.
     */
    explicit AccelerometerServer(int port, MainWindow *parent);

protected:
    /**
     * @brief incomingConnection is called whenever there is an incoming connection. This
     * method sets up the thread
     * @param socketDescriptor This is the socketDescriptor that is passed to the server
     */
    void incomingConnection(int socketDescriptor);
    MainWindow *parent;
    int portNumber;
    ServerThread *thread;

signals:

public slots:

};

#endif // ACCELEROMETERSERVER_H
