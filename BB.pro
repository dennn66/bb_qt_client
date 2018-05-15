#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T09:43:05
#
#-------------------------------------------------

QT       += core gui
#QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += winextras

TARGET = BB_5_12
TEMPLATE = app

win32:RC_FILE = bb.rc
include($$PWD/lib/QtTelegramBot/QtTelegramBot.pri)
include($$PWD/BadUSB/BadUSB.pri)
include($$PWD/L2Window/L2Window.pri)
include($$PWD/Interface/Interface.pri)

SOURCES += main.cpp \
    eventprocessor.cpp \

HEADERS  += \
    eventprocessor.h \

#FORMS    += \
#    keysettingsdialog.ui

LIBS += -lhid
LIBS += -lsetupapi
LIBS += -lgdi32
LIBS += -luser32


win32: LIBS += -L$$PWD/lib/winmm/ -lwinmm
INCLUDEPATH += $$PWD/lib/winmm
DEPENDPATH += $$PWD/lib/winmm
win32: PRE_TARGETDEPS += $$PWD/lib/winmm/libwinmm.a

