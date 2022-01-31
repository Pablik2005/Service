#-------------------------------------------------
#
# Project created by QtCreator 2017-02-22T11:23:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql xml network

TARGET = Service
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    class/clients.cpp \
    class/clientview.cpp \
    class/inifile.cpp \
    class/itemview.cpp \
    class/orderview.cpp \
    class/serviceview.cpp \
    class/optionsmysql.cpp \
    class/reportgenerator.cpp \
    class/sendsms.cpp \
    class/optionsmymobkit.cpp

HEADERS  += mainwindow.h \
    class/clients.h \
    class/clientview.h \
    class/inifile.h \
    class/itemview.h \
    class/orderview.h \
    class/serviceview.h \
    class/structures.h \
    class/optionsmysql.h \
    class/reportgenerator.h \
    class/sendsms.h \
    class/optionsmymobkit.h

FORMS    += mainwindow.ui \
    class/clients.ui \
    class/clientview.ui \
    class/itemview.ui \
    class/orderview.ui \
    class/serviceview.ui \
    class/optionsmysql.ui \
    class/reportgenerator.ui \
    class/sendsms.ui \
    class/optionsmymobkit.ui

RESOURCES += \
    res.qrc

RC_ICONS = moxum.ico
