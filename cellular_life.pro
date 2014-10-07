#-------------------------------------------------
#
# Project created by QtCreator 2014-10-04T21:18:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cellular_life
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    engine.cpp \
    thread_imp.cpp

QMAKE_CXXFLAGS += -std=c++11

QMAKE_LFLAGS += -pthread

HEADERS  += widget.h \
    engine.h \
    thread_imp.h

FORMS    += widget.ui
