!OBJ_READER_WRITER_PRI{
CONFIG += OBJ_READER_WRITER_PRI

INCLUDE += $$PWD

HEADERS += \
    $$PWD/basetypes.h \
    $$PWD/reader.h \
    $$PWD/writer.h

SOURCES += \
    $$PWD/basetypes.cpp \
    $$PWD/reader.cpp \
    $$PWD/writer.cpp
}
