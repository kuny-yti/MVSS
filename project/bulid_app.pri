
include (platform.pri)
#TRANSLATIONS =
DESTDIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/
UI_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.ui
MOC_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.moc
OBJECTS_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.obj
RCC_DIR = $${PWD}/bulid/$${BulidLevel}/$${Platform}/temp/$${TARGET}/.rcc

win32{
QMAKE_POST_LINK = copy /y $${PWD}\\bulid\\$${BulidLevel}\\$${Platform}\\*.dll $${PWD}\\bulid\\$${Platform}\\app\\
}
else:unix {
QMAKE_POST_LINK = cp -rf $${PWD}/bulid/$${BulidLevel}/$${Platform}/*.so* $${PWD}/bulid/$${BulidLevel}/$${Platform}/app/
}

