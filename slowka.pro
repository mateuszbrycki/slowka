#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T20:26:31
#
#-------------------------------------------------

QT       += core gui
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slowka
TEMPLATE = app
CONFIG += c++11

SOURCES += addnew.cpp \
 main.cpp \
 mainwindow.cpp \
 managewordlist.cpp \
 words.cpp

HEADERS += addnew.h \
 mainwindow.h \
 managewordlist.h \
 noEditDelegate.h \
 words.h


FORMS += addnew.ui \
 mainwindow.ui \
 managewordlist.ui

RESOURCES += images.qrc

OTHER_FILES +=

