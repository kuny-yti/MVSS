#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T00:38:08
#
#-------------------------------------------------

CONFIG -= qt
TARGET = render
TEMPLATE = lib

DEFINES += bsExportDll

include (../bulid_base.pri)
include (../include.pri)
include (./gui.pri)

INCLUDEPATH += $$PWD/../../source/render
VPATH += $$PWD/../../source/render

win32 {
  HEADERS += opengl/wgl/wgl_context.h
  SOURCES += opengl/wgl/wgl_context.cpp
  LIBS += -lopengl32 -lgdi32
}
else:unix{
  HEADERS += opengl/glx/glx_context.h
  SOURCES += opengl/glx/glx_context.cpp
  LIBS += -lX11 -lGL -lXrandr -lXinerama
}
DEFINES += GLEW_STATIC  GLEW_NO_GLU

OpenGL_GLEW_Header = \
    glew/GL/glew.h \
    glew/GL/eglew.h \
    glew/GL/glxew.h \
    glew/GL/wglew.h
OpenGL_GLEW_Source = \
     glew/glew.cpp

OpenGL_Source = \
    opengl/opengl_glu.cpp \
    opengl/opengl_pipeline_fixed.cpp \
    opengl/opengl_capability.cpp \
    opengl/opengl_buffer_object.cpp \
    opengl/opengl_frame_buffer.cpp \
    opengl/opengl_pixel_buffer.cpp \
    opengl/opengl_shader_program.cpp \
    opengl/opengl_texture.cpp \
    opengl/opengl_uniform_buffer.cpp \
    opengl/opengl_mx.cpp \
    opengl/opengl_vertex_array.cpp \
    opengl/opengl_vertex_buffer.cpp \
    opengl/opengl_indices_buffer.cpp

OpenGL_Header = \
    opengl/opengl_pipeline_fixed.h \
    opengl/opengl_capability.h \
    opengl/opengl_buffer_object.h \
    opengl/opengl_frame_buffer.h \
    opengl/opengl_pixel_buffer.h \
    opengl/opengl_shader_program.h \
    opengl/opengl_texture.h \
    opengl/opengl_uniform_buffer.h \
    opengl/opengl_glu.cpp \
    opengl/opengl_pipeline_fixed.cpp \
    opengl/opengl_capability.cpp \
    opengl/opengl_mx.h \
    opengl/opengl_state.h \
    opengl/opengl_context.h \
    opengl/opengl_vertex_array.h \
    opengl/opengl_vertex_buffer.h \
    opengl/opengl_indices_buffer.h

HEADERS += \
    $${OpenGL_Header} \
    $${OpenGL_GLEW_Header}

SOURCES += \
    $${OpenGL_Source} \
    $${OpenGL_GLEW_Source} \
    bs_render.cpp
