
INCLUDEPATH += $$PWD/../../source/render
VPATH += $$PWD/../../source/render

DEFINES += FONTSTASH_IMPLEMENTATION
DEFINES += STB_TRUETYPE_IMPLEMENTATION
DEFINES += GLFONTSTASH_IMPLEMENTATION

DEFINES += NANOVG_GL3_IMPLEMENTATION

#GUI panel
PanelHeader = \
    gui/panel/fontstash.h \
    gui/panel/stb_truetype.h \
    gui/panel/nanovg_gl.h \
    gui/panel/nanovg.h

PanelSource = \
    gui/panel/nanovg.cpp

#theme
ThemeHeader = \
    gui/theme/ui_themecore.h
ThemeSource = \
    gui/theme/ui_themecore.cpp

HEADERS += \
    $${PanelHeader} \
    $${ThemeHeader} \
    gui/core/ui_define.h \
    gui/core/ui_core.h  \
    gui/core/ui_layout.h \
    gui/core/ui_state.h \
    gui/core/ui_text.h \
    gui/core/ui_image.h \
    gui/core/ui_trigger.h \
    gui/core/ui_window.h \
    gui/bs_guiprivate.h


SOURCES += \
    $${PanelSource} \
    $${ThemeSource} \
    gui/core/ui_core.cpp  \
    gui/core/ui_layout.cpp \
    gui/core/ui_state.cpp \
    gui/core/ui_text.cpp \
    gui/core/ui_image.cpp \
    gui/core/ui_trigger.cpp \
    gui/core/ui_window.cpp \
    bs_gui.cpp
