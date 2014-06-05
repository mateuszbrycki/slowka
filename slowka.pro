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
 words.cpp \
    sqlite/sqlite3.c \
    models/modelwords.cpp

HEADERS += addnew.h \
 mainwindow.h \
 managewordlist.h \
 noEditDelegate.h \
 words.h \
    sqlite/sqlite3.h \
    models/modelwords.h


FORMS += addnew.ui \
 mainwindow.ui \
 managewordlist.ui

RESOURCES += images.qrc

OTHER_FILES +=

