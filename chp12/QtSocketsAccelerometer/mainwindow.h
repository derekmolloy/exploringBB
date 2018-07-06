/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include "accelerometerserver.h"
#include "sensorsample.h"

namespace Ui {
class MainWindow;
}

/**
 * This is a class declaration, otherwise there would be issues with circular declaracation
 * and the C++ single definition rule.
 */
class AccelerometerServer;

/**
 * @brief The MainWindow class is attached the GUI interface and controls all of the
 * components. The only input that is currently available is the exit button and the
 * menu.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * @brief on_exitButton_clicked is the slot that is called when the exit button is
     * clicked. It simply exits the program by closing the UI.
     */
    void on_exitButton_clicked();
    /**
     * @brief sampleConsume is an important slot as it is responsible for the update of the
     * GUI display. Whenever the XML parser identifies a Sensor sample, it creates a
     * SensorSample object and passes it to this slot. This slot calls the getter methods on
     * the object and updates the UI components.
     * @param sample The sample object that consists of simple pitch and roll values.
     */
    void sampleConsume(const SensorSample &sample);

private:
    Ui::MainWindow *ui;
    AccelerometerServer *server;   // The server that handles all socket traffic and creates the threads

    /**
     * @brief createActions this method only sets up the menu. It is called by the constructor.
     */
    void createActions();
};

#endif // MAINWINDOW_H
