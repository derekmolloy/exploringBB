/* A program for a Qt Temperature Sensor application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <sstream>
using namespace std;

#define LDR_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage"

// Constructor used to set up the default values for the created object
MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent), ui(new Ui::MainWindow)  {
   ui->setupUi(this);
   this->isFahrenheit = false;                 // default to Celsius
   this->isAlertSet = false;                   // no alert set
   statusBar()->showMessage("No alert set");   // status at bottom of window
   this->timer = new QTimer(this);             // create a new timer
     // when the timer times out, call the function on_updateTemperature()
   connect(timer, SIGNAL(timeout()), this, SLOT(on_updateTemperature()));
   this->maxTemperature = -100.0f;             // pick an impossible min
   this->minTemperature = 200.0f;              // pick an impossible max
   this->on_updateTemperature();               // explicitly update the display
   timer->start(5000);                         // set the time out to be 5 sec
}

MainWindow::~MainWindow() {                    // destructor, destroys UI
   delete ui;
}

void MainWindow::on_setAlertButton_clicked() { // set the alert temperature
   int sliderValue = ui->alertTempSlider->value();  // get the slider value
   if(sliderValue < getTemperature()){         // lower than current temp?
      QMessageBox::warning(this, "EBB Temperature",
                           "Alert setting too low!", QMessageBox::Discard);
   }
   else{                                       // alert value fine
      QString tempStr("Alert is set for: ");   // form a message
      tempStr.append(QString::number(sliderValue)); // with the alert temp
      statusBar()->showMessage(tempStr);       // display the message
      this->isAlertSet = true;                 // alert is set
      this->alertTemperature = sliderValue;    // alert temp set
   }
}

void MainWindow::on_isFahrenheitRadio_toggled(bool checked){
   this->isFahrenheit = checked;               // pressed the toggle button
   if(checked){
      ui->isCelsiusLabel->setText("Fahrenheit"); // display F on top-right
   }
   if(!checked){
      ui->isCelsiusLabel->setText("Celsius");  // display C on top-right
   }
   this->on_updateTemperature();               // update the temperature value
}

void MainWindow::on_clearAlertButton_clicked(){// remove the alert
   this->isAlertSet = false;
   statusBar()->showMessage("No alert set");
}

void MainWindow::on_alertTempSlider_valueChanged(int value) {
   ui->alertEdit->setText(QString::number(value)); // update alert text field
}

float MainWindow::getTemperature(){           // read the temperature
   int adc_value = readAnalog(0);             // get the ADC value
   float cur_voltage = adc_value * (1.80f/4096.0f);  // convert to voltage
   float diff_degreesC = (cur_voltage-0.75f)/0.01f;  // using TMP36 data sheet
   return(25.0f + diff_degreesC);             // return the temperature deg C
}

int MainWindow::readAnalog(int number){       // same as Chapter 10 code
   stringstream ss;
   ss << LDR_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

float MainWindow::celsiusToFahrenheit(float valueCelsius){
   return ((valueCelsius * (9.0f/5.0f)) + 32.0f);
}

void MainWindow::on_updateTemperature() {     // called whenever temp updated
   float temperature = this->getTemperature();
   QDateTime local(QDateTime::currentDateTime()); //display sample time
   statusBar()->showMessage(QString("Last update: ").append(local.toString()));
   if(temperature >= this->maxTemperature){   // is this the max temperature?
      this->maxTemperature = temperature;
      ui->maxEdit->setText(QString::number(temperature));
   }
   if(temperature <= this->minTemperature){   // is this the min temperature?
      this->minTemperature = temperature;
      ui->minEdit->setText(QString::number(temperature));
   }
   if(this->isFahrenheit){                    // is the display in Fahrenheit?
      ui->temperatureLCD->display((double)this->celsiusToFahrenheit(temperature));
   }
   else{                                      // must be Celsius
      ui->temperatureLCD->display((double)temperature);
   }
   if(this->isAlertSet){                      // is the alert enabled?
      if(temperature>=this->alertTemperature){   // does this exceed alert temp
         QString message("Alert! It is ");
         message.append(QString::number(temperature)).append(" degrees C!");
         QMessageBox::information(this, "EBB Temp", message, QMessageBox::Ok);
      }
   }
}
