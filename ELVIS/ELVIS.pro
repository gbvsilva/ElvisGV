#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T17:29:21
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ELVIS
TEMPLATE = app


SOURCES += main.cpp\
    GLWidget.cpp \
    drawFunctions.cpp \
    editFunctions.cpp \
    MainWindow.cpp

HEADERS  += \
    GLWidget.h \
    structs.h \
    drawFunctions.h \
    editFunctions.h \
    MainWindow.h

FORMS    += \
    MainWindow.ui

INCPATH = -I /usr/X11R6/include/ -I .
LIBS = -lglut -lGLU -lGL -lm #-lGLEW
LIBPATH = -L/usr/lib -L/usr/X11R6/lib/
