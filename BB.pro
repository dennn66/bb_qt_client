#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T09:43:05
#
#-------------------------------------------------

QT       += core gui
#QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += winextras

TARGET = BB_5_11
TEMPLATE = app

win32:RC_FILE = bb.rc
include($$PWD/lib/QtTelegramBot/QtTelegramBot.pri)
include($$PWD/lib/BadUSB/BadUSB.pri)

SOURCES += main.cpp \
    boredombreaker.cpp \
    #hid.c \
    #dongle.cpp \
    l2window.cpp \
    xpbar.cpp \
    keycondition.cpp \
    keysettingsdialog.cpp \
    keyconditionsset.cpp \
    systemkeyboardreadwrite.cpp \
    l2parser.cpp \
    clicker.cpp \
    systemmousehook.cpp \
    skillbar.cpp \
    barbox.cpp \
    l2graphicsobject.cpp \
    box.cpp \
    token.cpp \
    mainbox.cpp \
    mobbox.cpp \
    skill.cpp \
    skillstate.cpp \
    petbox.cpp \
    #donglekey.cpp \
    groupmanager.cpp \
    groupcondition.cpp \
    hotkeys.cpp \
    hotkey.cpp \
    telegrambot.cpp \
    resurectionbox.cpp \
    eventprocessor.cpp \
    targetbox.cpp \
    partybox.cpp

HEADERS  += \
    boredombreaker.h \
    #hidapi.h \
    #dongle.h \
    l2window.h \
    xpbar.h \
    keycondition.h \
    keysettingsdialog.h \
    keyconditionsset.h \
    systemkeyboardreadwrite.h \
    l2parser.h \
    clicker.h \
    systemmousehook.h \
    skillbar.h \
    barbox.h \
    l2graphicsobject.h \
    box.h \
    token.h \
    mainbox.h \
    mobbox.h \
    skill.h \
    skillstate.h \
    petbox.h \
    #donglekey.h \
    groupmanager.h \
    groupcondition.h \
    hotkeys.h \
    hotkey.h \
    telegrambot.h \
    resurectionbox.h \
    eventprocessor.h \
    targetbox.h \
    partybox.h

FORMS    += \
    boredombreaker.ui \
    keysettingsdialog.ui \
    clicker.ui

LIBS += -lhid
LIBS += -lsetupapi
LIBS += -lgdi32
LIBS += -luser32


win32: LIBS += -L$$PWD/lib/winmm/ -lwinmm
INCLUDEPATH += $$PWD/lib/winmm
DEPENDPATH += $$PWD/lib/winmm
win32: PRE_TARGETDEPS += $$PWD/lib/winmm/libwinmm.a

