INCLUDEPATH += $$PWD

#QT       += core network

SOURCES += \
    $$PWD/lib/hid/hid.c \
    $$PWD/dongle.cpp \
    $$PWD/donglekey.cpp

HEADERS += \
    $$PWD/lib/hid/hidapi.h \
    $$PWD/dongle.h \
    $$PWD/donglekey.h

OTHER_FILES += \
    $$PWD/README.md
