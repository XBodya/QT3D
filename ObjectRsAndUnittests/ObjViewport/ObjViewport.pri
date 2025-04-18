!OBJ_VIEWPORT_PRI{
CONFIG += OBJ_VIEWPORT_PRI

INCLUDE += $$PWD

HEADERS += \
    $$PWD/mesh.h \
    $$PWD/viewport.h \
    $$PWD/drawablegrid.h \
    $$PWD/drawableobject.h \
    $$PWD/draw.h

SOURCES += \
    $$PWD/mesh.cpp \
    $$PWD/viewport.cpp \
    $$PWD/drawablegrid.cpp \
    $$PWD/drawableobject.cpp \
    $$PWD/draw.cpp
}

!OBJ_VIEWPORT_TESTABLE_METHODS_PRI{
CONFIG += OBJ_VIEWPORT_TESTABLE_METHODS_PRI

INCLUDE += $$PWD

QT += openglwidgets
LIBS += -lopengl32

HEADERS += \
    $$PWD/mesh.h \
    $$PWD/drawablegrid.h \
    $$PWD/drawableobject.h \
    $$PWD/draw.h

SOURCES += \
    $$PWD/mesh.cpp \
    $$PWD/drawablegrid.cpp \
    $$PWD/drawableobject.cpp \
    $$PWD/draw.cpp


}
