#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T17:31:49
#
#-------------------------------------------------

QT       += widgets
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    simulation.cpp \
    graphic_blocks.cpp \
    scheme.cpp \
    logic_gui_interface.cpp \
    logic_blocks.cpp \
    logic_base.cpp \
    graphic_menus.cpp

HEADERS += \
    simulation.h \
    main.h \
    graphic_blocks.h \
    scheme.h \
    logic_gui_interface.h \
    logic_blocks.h \
    logic_base.h \
    graphic_menus.h

FORMS += \
        blockeditor.ui

RESOURCES += \
    buttondesign.qrc

