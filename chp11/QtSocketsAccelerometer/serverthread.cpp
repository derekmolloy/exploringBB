/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <QStatusBar>
#include <iostream>
#include <QXmlStreamReader>
#include "serverthread.h"
using namespace std;

ServerThread::ServerThread(int socketDescriptor, MainWindow *mainWindow, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
    QObject::connect(this, SIGNAL(sampleReceived(SensorSample)),
                     mainWindow, SLOT(sampleConsume(SensorSample)));
}

void ServerThread::run(){                  // the main thread loop
    QTcpSocket clientSocket;               // the clientSocket object
    this->running = true;                  // the main thread loop bool flag
    if (!clientSocket.setSocketDescriptor(socketDescriptor)){ //set up socket
        qDebug() << "Failed to setup the Socket";  // debug output
    }
    while(running){                        // loop forever until flag changes
       if(!clientSocket.waitForReadyRead(1000)){ // wait for up to 1 sec.
           this->running = false;                // failed - exit the loop
       }
       while(clientSocket.bytesAvailable()>0)    // are bytes available?
       {
           int x = clientSocket.bytesAvailable();  // how many?
           qDebug() << "There are " << x << " bytes available"; //debug
           char data[2000];                // capacity for up to 2000 bytes
           x = (qint64) clientSocket.read(&data[0], (qint64) x);
           data[x] = '\0';                 // add null in case of print output
           this->parse(&data[0]);          // parse the XML string
       }
    }
    clientSocket.close();                  // if loop finished, close socket
    qDebug() << "The client just disconnected";    // debug output
}


// XML string is in the form:
//  <sample><acc><pitch>2.44696</pitch><roll>-0.489249</roll></acc></sample> */
int ServerThread::parse(char *data){
  QXmlStreamReader xml(data);
  while(!xml.atEnd() && !xml.hasError()){
    if((xml.tokenType()==QXmlStreamReader::StartElement) &&    //found <sample>
       (xml.name()=="sample")){
      //This is a data sample <sample> - need to loop until </sample>
      float pitch = 0.0f, roll = 0.0f;
      xml.readNext();                                    // read next token
      while(!((xml.tokenType()==QXmlStreamReader::EndElement)&&    // </sample>
              (xml.name()=="sample")))
      {
        qDebug() << "Found a sample";
        if((xml.tokenType()==QXmlStreamReader::StartElement)       // <acc>
                  &&(xml.name()=="acc")){
          xml.readNext();
          qDebug() << "-- it has an acceleration element";
          while(!((xml.tokenType()==QXmlStreamReader::EndElement)  // </acc>
                    &&(xml.name()=="acc")))
          {
            if(xml.tokenType() == QXmlStreamReader::StartElement){ // <pitch>
              if(xml.name() == "pitch") {
                QString temp = xml.readElementText();    // read the value
                pitch = (temp).toFloat();                // convert to float
              }
              if(xml.name() == "roll") {                           // <roll>
                QString temp = xml.readElementText();
                roll = (temp).toFloat();
              }
            }
            xml.readNext();
          }
        }
        xml.readNext();
      }
      SensorSample sample(pitch, roll);      // create a sample object and
      emit sampleReceived(sample);           // emit it as a signal -- caught
      }                                      // by a slot in mainWindow, which
    xml.readNext();                          // updates the display widgets
    }
  return 0;
}
