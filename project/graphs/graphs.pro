#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T00:38:08
#
#-------------------------------------------------

CONFIG -= qt
TARGET = graphs
TEMPLATE = lib

DEFINES += bsExportDll

include (../bulid_base.pri)
include (../include.pri)


INCLUDEPATH += $$PWD/../../source/graphs
VPATH += $$PWD/../../source/graphs

Camera_Header = \
    camera/camera_control_impl.h \
    camera/camera_eye.h \
    camera/camera_impl.h \
    camera/camera_target.h \
    camera/frustum_impl.h
Camera_Source = \
    camera/camera_control_impl.cpp \
    camera/camera_eye.cpp \
    camera/camera_impl.cpp \
    camera/camera_target.cpp \
    camera/frustum_impl.cpp

HEADERS += \
    $${Camera_Header} \
    context_thread.h \
    ray_pick.h \
    window_base.h \
    graph_window.h

SOURCES += \
    $${Camera_Source} \
    context_thread.cpp \
    ray_pick.cpp \
    window_base.cpp \
    graph_window.cpp \
    bs_graphs.cpp
