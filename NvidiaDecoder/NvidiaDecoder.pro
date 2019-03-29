#-------------------------------------------------
#
# Project created by QtCreator 2018-12-28T14:12:22
#
#-------------------------------------------------

QT       -= core gui
CONFIG -= qt

TARGET = NvidiaDecoder
TEMPLATE = lib

DEFINES += NVDECODER_LIBRARY

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
    nvidiadecoder.cpp \
    NvDecoder.cpp

HEADERS += \
        nvdecoder_global.h \  
    nvidiadecoder.h \
    Utils/FFmpegDemuxer.h \
    Utils/FFmpegStreamer.h \
    Utils/Logger.h \
    Utils/NvCodecUtils.h \
    Utils/NvEncoderCLIOptions.h \
    cuviddec.h \
    nvcuvid.h \
    NvDecoder.h \
    decoder.h

win32{
contains(QT_ARCH, i386):{
    INCLUDEPATH += $$PWD/../3rd/ffmpeg/Win32/include \
                   $$(CUDA_PATH)/include

    LIBS += $$PWD/../3rd/ffmpeg/Win32/lib/avcodec.lib \
            $$PWD/../3rd/ffmpeg/Win32/lib/avformat.lib \
            $$PWD/../3rd/ffmpeg/Win32/lib/avutil.lib \
            $$PWD/../3rd/ffmpeg/Win32/lib/swresample.lib \
            $$PWD/../3rd/ffmpeg/Win32/lib/swscale.lib \
            $$(CUDA_PATH)/lib/Win32/cuda.lib \
            $$PWD/Lib/Win32/nvcuvid.lib
    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug/x86
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug/x86
}else{
    INCLUDEPATH += $$PWD/../3rd/ffmpeg/x64/include \
                   $$(CUDA_PATH)/include

    LIBS += $$PWD/../3rd/ffmpeg/x64/lib/avcodec.lib \
            $$PWD/../3rd/ffmpeg/x64/lib/avformat.lib \
            $$PWD/../3rd/ffmpeg/x64/lib/avutil.lib \
            $$PWD/../3rd/ffmpeg/x64/lib/swresample.lib \
            $$PWD/../3rd/ffmpeg/x64/lib/swscale.lib \
            $$(CUDA_PATH)/lib/x64/cuda.lib \
            $$PWD/Lib/x64/nvcuvid.lib
    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug/x64
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug/x64
}
}

unix {
INCLUDEPATH += /usr/local/cuda/include \
               /usr/local/ffmpeg/include
LIBS += -L/usr/local/ffmpeg/lib -lnvcuvid -lcuda \
        -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
    target.path = /usr/lib
    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug
}
