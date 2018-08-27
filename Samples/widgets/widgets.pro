#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T15:15:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = widgets
TEMPLATE = app

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
        main.cpp

HEADERS +=

win32{
contains(QT_ARCH, i386):{
INCLUDEPATH += $$PWD/../../package/include \
               $$PWD/../../3rd/ffmpeg/Win32/include
LIBS += $$PWD/../../package/lib/Win32/widget/klvideodecod.lib
}else{
INCLUDEPATH += $$PWD/../../package/include \
               $$PWD/../../3rd/ffmpeg/x64/include
LIBS += $$PWD/../../package/lib/x64/widget/klvideodecod.lib
}
}

unix{
INCLUDEPATH +=/usr/local/ffmpeg/include \
              /usr/local/klvideodecod/include

LIBS += -L$$PWD/../.. -lklvideodecod\
}