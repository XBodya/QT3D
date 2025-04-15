QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += openglwidgets
LIBS += -lopengl32

CONFIG += c++17

include(../ObjReader/ObjReader.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    viewport.cpp

HEADERS += \
    camera.h \
    mainwindow.h \
    mesh.h \
    viewport.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

DISTFILES += \
    ../ObjReader/airboat.obj \
    FragmentShader.frag \
    ObjViewport.pri \
    VertexShader.vert
