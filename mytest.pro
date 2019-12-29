#-------------------------------------------------
#
# Project created by QtCreator 2020-04-29T15:39:17
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mydb.cpp \
    osd_api.cpp \
    mysetting.cpp \
    mypower.cpp \
    myregister.cpp \
    myprogress.cpp \
    mylabel.cpp \
    floatpan.cpp

HEADERS  += mainwindow.h \
    mydb.h \
    osd_api.h \
    mysetting.h \
    mypower.h \
    myregister.h \
    myprogress.h \
    mylabel.h \
    floatpan.h

FORMS    += \
    mainwindow.ui \
    myregister.ui \
    floatpan.ui

QMAKE_LFLAGS += -Wl,--rpath=./

#unix:!macx: LIBS += -L$$PWD/lib_arm -ludev -lhidapi-hidraw
unix:!macx: LIBS += -L$$PWD/lib -ludev -lhidapi-hidraw
