#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T00:38:08
#
#-------------------------------------------------

CONFIG -= qt
TARGET = platform
TEMPLATE = lib

DEFINES += bsExportDll

include (../bulid_base.pri)
include (../include.pri)


INCLUDEPATH += $$PWD/../../source/platform
VPATH += $$PWD/../../source/platform

win32{

}
else:unix{
  INCLUDEPATH += $$PWD/../../source/platform/unix
  VPATH += $$PWD/../../source/platform/unix

  Unix_X11_ExtHeader = \
     x11/ext/x11_drag_drop.h \
     x11/ext/x11_clipboard.h \
     x11/ext/x11_window_manager.h \
     x11/ext/x11_inputmethod.h \
     x11/ext/x11_extension.h

  Unix_X11_ExtSource = \
     x11/ext/x11_drag_drop.cpp \
     x11/ext/x11_clipboard.cpp \
     x11/ext/x11_window_manager.cpp \
     x11/ext/x11_inputmethod.cpp \
     x11/ext/x11_extension.cpp

  Unix_X11_Header = \
     $${Unix_X11_ExtHeader} \
     x11/x11_header.h \
     x11/x11_event.h \
     x11/x11_monitor.h \
     x11/x11_cursor.h \
     x11/x11_window.h

  Unix_X11_Source = \
     $${Unix_X11_ExtSource} \
     x11/x11_event.cpp \
     x11/x11_monitor.cpp \
     x11/x11_cursor.cpp \
     x11/x11_window.cpp

  HEADERS += \
    $${Unix_X11_Header}

  SOURCES += \
    $${Unix_X11_Source} \
    bs_platform.cpp

  LIBS += -lX11 -lGL -lXrandr -lXinerama -lXxf86vm
}

