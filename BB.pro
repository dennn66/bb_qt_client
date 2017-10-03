#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T09:43:05
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
CONFIG       += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += winextras

TARGET = BBref9
TEMPLATE = app

win32:RC_FILE = bb.rc

SOURCES += \
    dongle/dongle.cpp \
    dongle/dongleworker.cpp \
    l2collection/l2window/l2parser/objects/barbox.cpp \
    l2collection/l2window/l2parser/objects/box.cpp \
    l2collection/l2window/l2parser/objects/l2graphicsobject.cpp \
    l2collection/l2window/l2parser/objects/mainbox.cpp \
    l2collection/l2window/l2parser/objects/mobbox.cpp \
    l2collection/l2window/l2parser/objects/petbox.cpp \
    l2collection/l2window/l2parser/objects/skill.cpp \
    l2collection/l2window/l2parser/objects/skillbar.cpp \
    l2collection/l2window/l2parser/objects/skillstate.cpp \
    l2collection/l2window/l2parser/objects/token.cpp \
    l2collection/l2window/l2parser/objects/xpbar.cpp \
    l2collection/l2window/l2parser/l2parser.cpp \
    l2collection/l2window/conditionmanager/keycondition.cpp \
    l2collection/l2window/conditionmanager/keyconditionsset.cpp \
    l2collection/l2window/conditionmanager/keysettingsdialog.cpp \
    l2collection/l2window/conditionmanager/conditionmanager.cpp \
    l2collection/l2collection.cpp \
    boredombreaker.cpp \
    clicker.cpp \
    hotkey.cpp \
    hotkeys.cpp \
    main.cpp \
    systemkeyboardreadwrite.cpp \
    systemmousehook.cpp \
    dongle/hid.c \
    l2collection/l2window/l2window.cpp \
    l2collection/l2collection_worker.cpp

HEADERS  += \
    dongle/dongle.h \
    dongle/dongleworker.h \
    dongle/hidapi.h \
    l2collection/l2window/l2parser/objects/barbox.h \
    l2collection/l2window/l2parser/objects/box.h \
    l2collection/l2window/l2parser/objects/l2graphicsobject.h \
    l2collection/l2window/l2parser/objects/mainbox.h \
    l2collection/l2window/l2parser/objects/mobbox.h \
    l2collection/l2window/l2parser/objects/petbox.h \
    l2collection/l2window/l2parser/objects/skill.h \
    l2collection/l2window/l2parser/objects/skillbar.h \
    l2collection/l2window/l2parser/objects/skillstate.h \
    l2collection/l2window/l2parser/objects/token.h \
    l2collection/l2window/l2parser/objects/xpbar.h \
    l2collection/l2window/l2parser/l2parser.h \
    l2collection/l2window/conditionmanager/keycondition.h \
    l2collection/l2window/conditionmanager/keyconditionsset.h \
    l2collection/l2window/conditionmanager/keysettingsdialog.h \
    l2collection/l2window/conditionmanager/conditionmanager.h \
    l2collection/l2collection.h \
    l2collection/l2window/l2window.h \
    l2collection/l2collection_worker.h\
    boredombreaker.h \
    clicker.h \
    hotkey.h \
    hotkeys.h \
    systemkeyboardreadwrite.h \
    systemmousehook.h

FORMS    += \
    l2collection/l2window/conditionmanager/keysettingsdialog.ui \
    boredombreaker.ui \
    clicker.ui

LIBS += -lhid
LIBS += -lsetupapi
LIBS += -lgdi32
LIBS += -luser32

