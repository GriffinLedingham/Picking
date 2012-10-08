#-------------------------------------------------
#
# Project created by QtCreator 2012-09-13T13:41:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Picking
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    basicopenglview.cpp \
    catmullrominterpolation.cpp

HEADERS  += mainwindow.h \
    basicopenglview.h \
    catmullrominterpolation.h

FORMS    += mainwindow.ui
