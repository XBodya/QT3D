QT += testlib
QT += gui

include(../ObjReader/ObjReader.pri)

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_testreader.cpp
