#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T00:38:08
#
#-------------------------------------------------

CONFIG -= qt
TARGET = gui
TEMPLATE = lib

DEFINES += bsExportDll

include (../bulid_base.pri)
include (../include.pri)


INCLUDEPATH += $$PWD/../../source/gui
VPATH += $$PWD/../../source/gui



SOURCES += \
   bs_gui.cpp
