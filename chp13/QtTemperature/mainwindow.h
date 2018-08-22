/* A program for a Qt Temperature Sensor Application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>      // for the main GUI interface
#include <QTimer>           // for a timer that periodically reads the temp
#include <QDateTime>        // to find out the date/time of the sample
#include <QMessageBox>      // for pop-up message boxes - e.g., alert!

namespace Ui {
class MainWindow;           // different way of placing class in UI namespace
}

class MainWindow : public QMainWindow
{
    // This macro must appear in the private section of a class that
    // declares its own signals and slots
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);      // constructor
    ~MainWindow();                                 // destructor

private slots:
    void on_setAlertButton_clicked();
    void on_isFahrenheitRadio_toggled(bool checked);
    void on_clearAlertButton_clicked();
    void on_alertTempSlider_valueChanged(int value);  //if alert slider moved
    void on_updateTemperature();  // this slot is triggered by the timer

private:
    Ui::MainWindow *ui;           // the main user interface
    QTimer *timer;                // timer thread that triggers after a delay
    bool  isFahrenheit;           // is the main display deg C or deg F
    int   alertTemperature;       // the alert temperature value
    bool  isAlertSet;             // is the alert set?
    float minTemperature, maxTemperature;     // min and max temp values
    float celsiusToFahrenheit(float valueCelsius);  //function for conversion
    float getTemperature();       // get the temperature from the sensor
    int   readAnalog(int number); // used by getTemperature to read the ADC
};

#endif // MAINWINDOW_H
