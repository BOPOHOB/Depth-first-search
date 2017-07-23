#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T12:05:36
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bojko2
TEMPLATE = app


SOURCES += main.cpp\
        gui.cpp \
    kernel/engine.cpp
SOURCES += kernel/cbasicmatrix.cpp

HEADERS  += gui.h \
    kernel/engine.h
HEADERS += kernel/cbasicmatrix.h
