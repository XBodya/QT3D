QT += testlib
QT += gui
QT += widgets

CONFIG += OBJ_VIEWPORT_PRI
include(../ObjViewport/ObjViewport.pri)

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testmesh.cpp
