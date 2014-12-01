/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include "mainwindow.h"
#include "sensorsample.h"

class MainWindow;

/**
 * @brief The ServerThread class is instantiated by the server class whenever there is a
 * new socket connection. It is important that the socket data is parsed by a thread, as
 * otherwise the UI would be unresponsive, as the main thread would be occupied with the
 * socket data transfer. This class extends QThread.
 */
class ServerThread : public QThread
{
    Q_OBJECT
    int socketDescriptor;  // sent from the AccelerometerServer class

public:
    /**
     * @brief ServerThread the constructor for the class
     * @param socketDescriptor the socket descriptor that is passed from the server class
     * @param mainWindow a reference to the main window so that the signal can be connected
     * @param parent a reference to the parent class QWidget
     */
    explicit ServerThread(int socketDescriptor, MainWindow *mainWindow, QObject *parent = 0);

    /**
     * @brief run the main thread loop
     */
    void run();

signals:
    /**
     * @brief sampleReceived a signal that is generated when a <sample> is successfully parsed
     * and an object has been created
     * @param sample the data object that is passed to the slot after successful parse
     */
    void sampleReceived(const SensorSample &sample);

public slots:

private:
    /**
     * @brief parse the XML parser for the class
     * @return 0 if there is a successful parse (not currently used)
     */
    int parse(char *);
    /**
     * @brief running the main thread running state. Code could be added to control this state
     * and externally request a graceful exit of the main thread loop.
     */
    bool running;
};

#endif // SERVERTHREAD_H
