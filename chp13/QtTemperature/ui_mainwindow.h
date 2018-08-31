/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(405, 168);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        temperatureLCD = new QLCDNumber(centralWidget);
        temperatureLCD->setObjectName(QStringLiteral("temperatureLCD"));
        temperatureLCD->setGeometry(QRect(110, 10, 201, 81));
        temperatureLCD->setSmallDecimalPoint(true);
        temperatureLCD->setDigitCount(6);
        temperatureLCD->setSegmentStyle(QLCDNumber::Filled);
        temperatureLCD->setProperty("value", QVariant(25.5));
        setAlertButton = new QPushButton(centralWidget);
        setAlertButton->setObjectName(QStringLiteral("setAlertButton"));
        setAlertButton->setGeometry(QRect(320, 100, 75, 23));
        minEdit = new QLineEdit(centralWidget);
        minEdit->setObjectName(QStringLiteral("minEdit"));
        minEdit->setGeometry(QRect(40, 10, 61, 20));
        minEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        minEdit->setReadOnly(true);
        isCelsiusLabel = new QLabel(centralWidget);
        isCelsiusLabel->setObjectName(QStringLiteral("isCelsiusLabel"));
        isCelsiusLabel->setGeometry(QRect(320, 10, 61, 21));
        minLabel = new QLabel(centralWidget);
        minLabel->setObjectName(QStringLiteral("minLabel"));
        minLabel->setGeometry(QRect(10, 10, 21, 16));
        maxEdit = new QLineEdit(centralWidget);
        maxEdit->setObjectName(QStringLiteral("maxEdit"));
        maxEdit->setGeometry(QRect(40, 40, 61, 20));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(maxEdit->sizePolicy().hasHeightForWidth());
        maxEdit->setSizePolicy(sizePolicy);
        maxEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxEdit->setReadOnly(true);
        maxLabel = new QLabel(centralWidget);
        maxLabel->setObjectName(QStringLiteral("maxLabel"));
        maxLabel->setGeometry(QRect(10, 40, 31, 16));
        isFahrenheitRadio = new QRadioButton(centralWidget);
        isFahrenheitRadio->setObjectName(QStringLiteral("isFahrenheitRadio"));
        isFahrenheitRadio->setGeometry(QRect(320, 50, 82, 17));
        alertTempSlider = new QSlider(centralWidget);
        alertTempSlider->setObjectName(QStringLiteral("alertTempSlider"));
        alertTempSlider->setGeometry(QRect(110, 100, 201, 21));
        alertTempSlider->setMaximum(50);
        alertTempSlider->setSliderPosition(25);
        alertTempSlider->setOrientation(Qt::Horizontal);
        alertEdit = new QLineEdit(centralWidget);
        alertEdit->setObjectName(QStringLiteral("alertEdit"));
        alertEdit->setGeometry(QRect(10, 100, 91, 20));
        alertEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        alertEdit->setReadOnly(true);
        alertLabel = new QLabel(centralWidget);
        alertLabel->setObjectName(QStringLiteral("alertLabel"));
        alertLabel->setGeometry(QRect(10, 80, 101, 16));
        clearAlertButton = new QPushButton(centralWidget);
        clearAlertButton->setObjectName(QStringLiteral("clearAlertButton"));
        clearAlertButton->setGeometry(QRect(320, 70, 75, 23));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 405, 21));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QStringLiteral("menuMenu"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EBB Temperature Sensor", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        setAlertButton->setText(QApplication::translate("MainWindow", "Set Alert", Q_NULLPTR));
        isCelsiusLabel->setText(QApplication::translate("MainWindow", "Celsius", Q_NULLPTR));
        minLabel->setText(QApplication::translate("MainWindow", "Min:", Q_NULLPTR));
        maxLabel->setText(QApplication::translate("MainWindow", "Max:", Q_NULLPTR));
        isFahrenheitRadio->setText(QApplication::translate("MainWindow", "Fahrenheit", Q_NULLPTR));
        alertEdit->setText(QApplication::translate("MainWindow", "25", Q_NULLPTR));
        alertLabel->setText(QApplication::translate("MainWindow", "Alert Temperature:", Q_NULLPTR));
        clearAlertButton->setText(QApplication::translate("MainWindow", "Clear Alert", Q_NULLPTR));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
