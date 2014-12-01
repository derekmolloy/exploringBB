/* A program for a Qt Socket-based Temperature Application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QHostAddress>
using namespace std;

#define LDR_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  {
    ui->setupUi(this);
    this->dialog = new ServerSettingsDialog(this);
    this->isFahrenheit = false;
    this->isAlertSet = false;
    statusBar()->showMessage("No alert set");
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_updateTemperature()));
    this->curTemperature = 25.0f;
    this->maxTemperature = -100.0f;
    this->minTemperature = 200.0f;
    this->on_updateTemperature();
    timer->start(5000);
    this->createActions();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_setAlertButton_clicked() {
    int sliderValue = ui->alertTempSlider->value();
    this->getSensorTemperature();
    if(sliderValue < this->curTemperature){
        QMessageBox::warning(this, "EBB Temperature", "Alert setting too low!", QMessageBox::Discard);
    }
    else{
        QString tempStr("Alert is set for: ");
        tempStr.append(QString::number(sliderValue));
        statusBar()->showMessage(tempStr);
        this->isAlertSet = true;
        this->alertTemperature = sliderValue;
    }
}

void MainWindow::on_isFahrenheitRadio_toggled(bool checked){
    this->isFahrenheit = checked;
    if(checked){
        ui->isCelsiusLabel->setText("Fahrenheit");
    }
    if(!checked){
        ui->isCelsiusLabel->setText("Celsius");
    }
    this->on_updateTemperature();
}

void MainWindow::on_clearAlertButton_clicked(){
    this->isAlertSet = false;
    statusBar()->showMessage("No alert set");
}

void MainWindow::on_alertTempSlider_valueChanged(int value) {
    ui->alertEdit->setText(QString::number(value));
}

int MainWindow::getSensorTemperature(){
   // Get the server address and port from the settings dialog box
   int serverPort = this->dialog->getServerPort();  // get from the dialog box
   quint32 serverAddr = this->dialog->getIPAddress();   // from the dialog box
   QTcpSocket *tcpSocket = new QTcpSocket(this);    // create socket
   tcpSocket->connectToHost(QHostAddress(serverAddr), serverPort); // connect
   if(!tcpSocket->waitForConnected(1000)){    //wait up to 1s for a connection
      statusBar()->showMessage("Failed to connect to server...");
      return 1;
   }
   // Send the message "getTemperature" to the server
   tcpSocket->write("getTemperature");
   if(!tcpSocket->waitForReadyRead(1000)){    // wait up to 1s for the server
      statusBar()->showMessage("Server did not respond...");
      return 1;
   }
   // If the server has sent bytes back to the client
   if(tcpSocket->bytesAvailable()>0){
      int size = tcpSocket->bytesAvailable(); // how many bytes are ready?
      char data[20];                          // upper limit of 20 chars
      tcpSocket->read(&data[0],(qint64)size); // read the number of bytes rec.
      data[size]='\0';                        // termintate the string
      this->curTemperature = atof(data);      // string -> float conversion
      cout << "Received the data [" << this->curTemperature << "]" << endl;
   }
   else{
      statusBar()->showMessage("No data available...");
   }
   return 0;    // the on_updateTemperature() slot will update the display
}

float MainWindow::celsiusToFahrenheit(float valueCelsius){
    return ((valueCelsius * (9.0f/5.0f)) + 32.0f);
}

void MainWindow::createActions(){
    QAction *exit = new QAction("&Exit", this);
    exit->setShortcut(QKeySequence(tr("Ctrl+X")));
    QAction *settings = new QAction("&Settings", this);
    settings->setShortcut(QKeySequence(tr("Ctrl+S")));
    QMenu *menu = menuBar()->addMenu("&Menu");
    menu->addAction(settings);
    menu->addAction(exit);
    connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));  //quit application
    connect(settings, SIGNAL(triggered()), this, SLOT(on_openSettings()));
}

void MainWindow::on_openSettings(){
    this->dialog->exec();                             // display the dialog box
    this->timer->start(1000*this->dialog->getTimeDelay()); //update timer delay
}

void MainWindow::on_updateTemperature() {
    //Get the current date and time on the client machine
    QDateTime local(QDateTime::currentDateTime());
    statusBar()->showMessage(QString("Last update: ").append(local.toString()));
    //Get the temperature from the server
    this->getSensorTemperature();
    //Is it the maximum? If so, update the maximum
    if(this->curTemperature >= this->maxTemperature){
        this->maxTemperature = this->curTemperature;
        ui->maxEdit->setText(QString::number(this->curTemperature,'f',3));
    }
    //Is it the minimum? If so, update the minimum
    if(this->curTemperature <= this->minTemperature){
        this->minTemperature = this->curTemperature;
        ui->minEdit->setText(QString::number(this->curTemperature,'f',3));
    }
    //Is the display in Fahrenheit? If so, convert and update the display
    if(this->isFahrenheit){
        ui->temperatureLCD->display((double)this->celsiusToFahrenheit(this->curTemperature));
    }
    else{ //In Celsius
        ui->temperatureLCD->display((double)this->curTemperature);
    }
    //Does this new value trigger the alert? If so, pop up a message box.
    if(this->isAlertSet){
        if(this->curTemperature>=this->alertTemperature){
            QString message("Alert! It is ");
            message.append(QString::number(this->curTemperature)).append(" degrees Celsius!");
            QMessageBox::information(this, "EBB Temperature", message, QMessageBox::Ok);
        }
    }
}
