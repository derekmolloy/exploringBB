/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Aug 5 19:20:31 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QLCDNumber *temperatureLCD;
    QPushButton *setAlertButton;
    QLineEdit *minEdit;
    QLabel *isCelsiusLabel;
    QLabel *minLabel;
    QLineEdit *maxEdit;
    QLabel *maxLabel;
    QRadioButton *isFahrenheitRadio;
    QSlider *alertTempSlider;
    QLineEdit *alertEdit;
    QLabel *alertLabel;
    QPushButton *clearAlertButton;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuMenu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(405, 168);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        temperatureLCD = new QLCDNumber(centralWidget);
        temperatureLCD->setObjectName(QString::fromUtf8("temperatureLCD"));
        temperatureLCD->setGeometry(QRect(110, 10, 201, 81));
        temperatureLCD->setSmallDecimalPoint(true);
        temperatureLCD->setDigitCount(6);
        temperatureLCD->setSegmentStyle(QLCDNumber::Filled);
        temperatureLCD->setProperty("value", QVariant(25.5));
        setAlertButton = new QPushButton(centralWidget);
        setAlertButton->setObjectName(QString::fromUtf8("setAlertButton"));
        setAlertButton->setGeometry(QRect(320, 100, 75, 23));
        minEdit = new QLineEdit(centralWidget);
        minEdit->setObjectName(QString::fromUtf8("minEdit"));
        minEdit->setGeometry(QRect(40, 10, 61, 20));
        minEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        minEdit->setReadOnly(true);
        isCelsiusLabel = new QLabel(centralWidget);
        isCelsiusLabel->setObjectName(QString::fromUtf8("isCelsiusLabel"));
        isCelsiusLabel->setGeometry(QRect(320, 10, 61, 21));
        minLabel = new QLabel(centralWidget);
        minLabel->setObjectName(QString::fromUtf8("minLabel"));
        minLabel->setGeometry(QRect(10, 10, 21, 16));
        maxEdit = new QLineEdit(centralWidget);
        maxEdit->setObjectName(QString::fromUtf8("maxEdit"));
        maxEdit->setGeometry(QRect(40, 40, 61, 20));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(maxEdit->sizePolicy().hasHeightForWidth());
        maxEdit->setSizePolicy(sizePolicy);
        maxEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxEdit->setReadOnly(true);
        maxLabel = new QLabel(centralWidget);
        maxLabel->setObjectName(QString::fromUtf8("maxLabel"));
        maxLabel->setGeometry(QRect(10, 40, 31, 16));
        isFahrenheitRadio = new QRadioButton(centralWidget);
        isFahrenheitRadio->setObjectName(QString::fromUtf8("isFahrenheitRadio"));
        isFahrenheitRadio->setGeometry(QRect(320, 50, 82, 17));
        alertTempSlider = new QSlider(centralWidget);
        alertTempSlider->setObjectName(QString::fromUtf8("alertTempSlider"));
        alertTempSlider->setGeometry(QRect(110, 100, 201, 21));
        alertTempSlider->setMaximum(50);
        alertTempSlider->setSliderPosition(25);
        alertTempSlider->setOrientation(Qt::Horizontal);
        alertEdit = new QLineEdit(centralWidget);
        alertEdit->setObjectName(QString::fromUtf8("alertEdit"));
        alertEdit->setGeometry(QRect(10, 100, 91, 20));
        alertEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        alertEdit->setReadOnly(true);
        alertLabel = new QLabel(centralWidget);
        alertLabel->setObjectName(QString::fromUtf8("alertLabel"));
        alertLabel->setGeometry(QRect(10, 80, 101, 16));
        clearAlertButton = new QPushButton(centralWidget);
        clearAlertButton->setObjectName(QString::fromUtf8("clearAlertButton"));
        clearAlertButton->setGeometry(QRect(320, 70, 75, 23));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 405, 21));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EBB Temperature Sensor", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        setAlertButton->setText(QApplication::translate("MainWindow", "Set Alert", 0, QApplication::UnicodeUTF8));
        isCelsiusLabel->setText(QApplication::translate("MainWindow", "Celsius", 0, QApplication::UnicodeUTF8));
        minLabel->setText(QApplication::translate("MainWindow", "Min:", 0, QApplication::UnicodeUTF8));
        maxLabel->setText(QApplication::translate("MainWindow", "Max:", 0, QApplication::UnicodeUTF8));
        isFahrenheitRadio->setText(QApplication::translate("MainWindow", "Fahrenheit", 0, QApplication::UnicodeUTF8));
        alertEdit->setText(QApplication::translate("MainWindow", "25", 0, QApplication::UnicodeUTF8));
        alertLabel->setText(QApplication::translate("MainWindow", "Alert Temperature:", 0, QApplication::UnicodeUTF8));
        clearAlertButton->setText(QApplication::translate("MainWindow", "Clear Alert", 0, QApplication::UnicodeUTF8));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
