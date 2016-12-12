#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T00:38:08
#
#-------------------------------------------------

CONFIG -= qt
TARGET = base
TEMPLATE = lib

DEFINES += bsExportDll

include (../bulid_base.pri)
include(google.pri)

win32 {
LIBS+=  -lws2_32 -lwsock32 -lz
}
unix:!macx {
LIBS += -lz -ldl

}

DEFINES += YAML_DECLARE_STATIC

INCLUDEPATH += $$PWD/../../source/base
VPATH += $$PWD/../../source/base

INCLUDEPATH += $${PWD}/../../include
VPATH += $${PWD}/../../include
INCLUDEPATH += $${PWD}/../../include/maths
VPATH += $${PWD}/../../include/maths
INCLUDEPATH += $${PWD}/../../include/tools
VPATH += $${PWD}/../../include/tools

DEFINES += STB_IMAGE_STATIC
DEFINES += STB_IMAGE_WRITE_STATIC
DEFINES += STB_IMAGE_IMPLEMENTATION
DEFINES += STB_IMAGE_WRITE_IMPLEMENTATION

QMAKE_CFLAGS += -fshort-wchar -fPIC
#QMAKE_CFLAGS += -msse4a -mavx2 -mavx -mbmi2 -mbmi -msse2 -mf16c -mfma4 -mfma -mrdrnd -mfsgsbase -mlwp -mlzcnt -m3dnow -mmmx -msse4.2 -msse3 -mpopcnt -msse4 -mtbm -mssse3 -maes -mpclmul -msse -mxop

QMAKE_CXXFLAGS += -fshort-wchar -fPIC
#QMAKE_CXXFLAGS += -msse4a -mavx2 -mavx -mbmi2 -mbmi -msse2 -mf16c -mfma4 -mfma -mrdrnd -mfsgsbase -mlwp -mlzcnt -m3dnow -mmmx -msse4.2 -msse3 -mpopcnt -msse4 -mtbm -mssse3 -maes -mpclmul -msse -mxop
CONFIG += C++14

# yaml
YamlHeader = \
    yaml/config.h \
    yaml/yaml.h \
    yaml/yaml_private.h

YamlSource = \
    yaml/api.c \
    yaml/dumper.c \
    yaml/emitter.c \
    yaml/loader.c \
    yaml/parser_in.c \
    yaml/reader_.c \
    yaml/scanner.c \
    yaml/writer.c

ImplHeader = \
    impl/impl_dynamic.h \
    impl/impl_file.h \
    impl/impl_directory.h \
    impl/impl_archive.h

ImplSource = \
    impl/impl_dynamic.cpp \
    impl/impl_file.cpp \
    impl/impl_directory.cpp \
    impl/impl_archive.cpp

ZipHeader = \
    zip/unzip.h \
    zip/zip.h \
    zip/ioapi.h \
    zip/crypt.h

ZipSource = \
    zip/unzip.c \
    zip/zip.c \
    zip/ioapi.c

StbHeader = \
    stb/stb_image.h \
    stb/stb_image_resize.h \
    stb/stb_image_write.h

SOURCES += \
    $${YamlSource} \
    $${ImplSource} \
    $${ZipSource}\
    unicode.cpp \
    bs_image.cpp \
    bs_thread.cpp \
    bs_datetime.cpp \
    bs_timer.cpp \
    bs_string.cpp \
    bs_char.cpp \
    bs_md5.cpp \
    bs_uuid.cpp \
    bs_url.cpp \
    bs_memory.cpp \
    bs_parser.cpp \
    bs_stream.cpp \
    bs_array.cpp \
    bs_list.cpp \
    bs_map.cpp \
    bs_maphash.cpp \
    bs_object.cpp \
    bs_filesytem.cpp \
    bs_debug.cpp \
    bs_application.cpp

HEADERS += \
    $${YamlHeader} \
    $${ImplHeader} \
    $${ZipHeader} \
    $${StbHeader} \
    unicode.h \
    unicode_table.h \
    bs_ucs.h \
    bs_ucsprivate.h \
    bs_number_double.h \
    bs_number_longlong.h



