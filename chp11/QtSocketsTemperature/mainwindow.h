#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QTcpSocket>
#include "serversettingsdialog.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class is linked to the main user interface of the
 * application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);        // called by the main.cpp file
    ~MainWindow();

private slots:
    void on_setAlertButton_clicked();                // the alert button is pressed
    void on_isFahrenheitRadio_toggled(bool checked); // the toggle button is pressed
    void on_clearAlertButton_clicked();              // the clear alert button is clicked
    void on_alertTempSlider_valueChanged(int value); // the slider is moved
    void on_updateTemperature();                     // called by the timer to update the temperature
    void on_openSettings();                          // opens the server settings window

private:
    Ui::MainWindow *ui;               // The main window UI
    QTimer *timer;                    // The timer that updates the temperature
    ServerSettingsDialog *dialog;     // The Server settings dialog

    bool  isFahrenheit;
    int   alertTemperature;
    bool  isAlertSet;
    float curTemperature, minTemperature, maxTemperature;
    float celsiusToFahrenheit(float valueCelsius);
    int   getSensorTemperature();     // gets the temperature from the server

    void createActions();             // sets up the menu for the window
};

#endif // MAINWINDOW_H
