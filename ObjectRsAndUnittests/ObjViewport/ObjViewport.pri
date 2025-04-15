!OBJ_VIEWPORT_PRI{
CONFIG += OBJ_VIEWPORT_PRI

INCLUDE += $$PWD

HEADERS += \
    $$PWD/mesh.h \
    $$PWD/viewport.h

SOURCES += \
    $$PWD/mesh.cpp \
    $$PWD/viewport.cpp
}

!OBJ_VIEWPORT_TESTABLE_METHODS_PRI{
CONFIG += OBJ_VIEWPORT_TESTABLE_METHODS_PRI

INCLUDE += $$PWD

HEADERS += \
    $$PWD/mesh.h

SOURCES += \
    $$PWD/mesh.cpp
}
