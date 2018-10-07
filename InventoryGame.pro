#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T20:50:26
#
#-------------------------------------------------

QT       += core gui sql multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InventoryGame
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
    ig_inventorytable.cpp \
    ig_slot.cpp \
    ig_item.cpp \
    ig_databasemanager.cpp \
    ig_MainWidget.cpp \
    ig_MainMenu.cpp \
    ig_server.cpp

HEADERS += \
    ig_inventorytable.h \
    ig_slot.h \
    ig_item.h \
    ig_databasemanager.h \
    ig_MainWidget.h \
    ig_MainMenu.h \
    ig_server.h \
    ig_consts.h

FORMS += \
        ig_mainwidget.ui \
    ig_mainmenu.ui

RESOURCES += \
    sourses.qrc
