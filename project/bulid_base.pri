include (platform.pri)

DESTDIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/
UI_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.ui
MOC_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.moc
OBJECTS_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.obj
RCC_DIR  = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.rcc

QMAKE_CFLAGS += -W
QMAKE_CXXFLAGS += -W
