#-------------------------------------------------
#
# Project created by QtCreator 2018-12-28T19:28:26
#
#-------------------------------------------------

QT       += widgets

TARGET = klvideowidget
TEMPLATE = lib

DEFINES += KLVIDEOWIDGET_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        klvideowidget.cpp \
    utils/nv12render.cpp \
    utils/rendermanager.cpp \
    utils/videodata.cpp \
    utils/yuvrender.cpp \
    videodatacache.cpp

HEADERS += \
        klvideowidget.h \
        klvideowidget_global.h \
    utils/klvideorender.h \
    utils/nv12render.h \
    utils/rendermanager.h \
    utils/videodata.h \
    utils/yuvrender.h \
    decoder.h \
    videodatacache.h

win32{
contains(QT_ARCH, i386):{
    INCLUDEPATH += $$PWD/../3rd/ffmpeg/Win32/include

    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug/Win32
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug/Win32
}else{
    INCLUDEPATH += $$PWD/../3rd/ffmpeg/x64/include

    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug/x64
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug/x64
}
}

unix {
INCLUDEPATH += /usr/local/ffmpeg/include
    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug
}
