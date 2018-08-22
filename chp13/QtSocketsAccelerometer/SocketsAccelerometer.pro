#-------------------------------------------------
#
# Project created by QtCreator 2014-08-09T02:19:40
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SocketsAccelerometer
TEMPLATE = app


SOURCES += \
        main.cpp\
        mainwindow.cpp \
    serverthread.cpp \
    accelerometerserver.cpp \
    sensorsample.cpp


HEADERS  += mainwindow.h \
    serverthread.h \
    accelerometerserver.h \
    sensorsample.h

FORMS    += mainwindow.ui
