INCLUDEPATH += $$PWD

include($$PWD/GroupMgr/GroupMgr.pri)
include($$PWD/L2Mgr/L2Mgr.pri)
include($$PWD/ConditionMgr/ConditionMgr.pri)

SOURCES +=  \
    $$PWD/l2window.cpp \
    $$PWD/l2parser.cpp \

HEADERS  += \
    $$PWD/l2window.h \
    $$PWD/l2parser.h \
