#-------------------------------------------------
#
# Project created by QtCreator 2014-07-19T17:08:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Temperature
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h

FORMS    += mainwindow.ui \
    serversettingsdialog.ui
