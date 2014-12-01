/* A Qt-based Accelerometer application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int portNumber = 4444;
    this->server = new AccelerometerServer(portNumber, this);
    if (!server->listen(QHostAddress::Any,(quint16)portNumber)) {
        QMessageBox::critical(this, tr("Accelerometer Server"),
                              tr("Failed to start: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }
    else{
        statusBar()->showMessage("Server started on port 4444.");
    }
    this->createActions();
}

void MainWindow::sampleConsume(const SensorSample &sample){
   SensorSample s = sample;
   qDebug() << "Consuming a sample: " << s.getPitch() << ", " << s.getRoll() << endl;
   ui->pitchDial->setValue((int)s.getPitch());
   ui->rollDial->setValue((int)s.getRoll());
   ui->pitchSlider->setValue((int)s.getPitch());
   ui->rollSlider->setValue((int)s.getRoll());
   ui->pitchSpinBox->setValue(s.getPitch());
   ui->rollSpinBox->setValue(s.getRoll());
   //Get the current date and time on the client machine
   QDateTime local(QDateTime::currentDateTime());
   statusBar()->showMessage(QString("Last update: ").append(local.toString()));
}

void MainWindow::createActions(){
    QAction *exit = new QAction("&Exit", this);
    exit->setShortcut(QKeySequence(tr("Ctrl+X")));
    QMenu *menu = menuBar()->addMenu("&Menu");
    menu->addAction(exit);
    connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));  //quit application
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}
