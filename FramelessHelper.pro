#-------------------------------------------------
#
# Project created by QtCreator 2017-08-22T16:25:35
#
#-------------------------------------------------

QT += qml quick widgets

TARGET   = StyledWindow
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

include(FramelessHelper/FramelessHelper.pri)

SOURCES += \
        main.cpp

HEADERS += \
    MainWindow.h
FORMS += \
    MainWindow.ui
SOURCES += \
    MainWindow.cpp
RESOURCES += \
    FramelessHelper.qrc
