#-------------------------------------------------
#
# Project created by QtCreator 2018-08-17T10:56:11
#
#-------------------------------------------------

#QT       -= gui
#QT       +=  quick
CONFIG -= qt

TARGET = klvideodecod
TEMPLATE = lib

DEFINES += KLVIDEODECOD_LIBRARY

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
    NvDecoder/NvDecoder.cpp \
    NvDecoder/nvidiadecoder.cpp \
    fmgdecoder/ffmpegdecoder.cpp \
#    QmlVideo/videorender.cpp \
#    Videowidget/klvideowidget.cpp \
#    Videowidget/videodatacache.cpp \
#    utils/rendermanager.cpp \
#    utils/videodata.cpp \
#    utils/nv12render.cpp \
#    utils/yuvrender.cpp

HEADERS += \
        klvideodecod_global.h \ 
    decoder.h \
    NvDecoder/cuviddec.h \
    NvDecoder/nvcuvid.h \
    NvDecoder/NvDecoder.h \
    NvDecoder/nvidiadecoder.h \
    NvDecoder/Utils/FFmpegDemuxer.h \
    NvDecoder/Utils/FFmpegStreamer.h \
    NvDecoder/Utils/Logger.h \
    NvDecoder/Utils/NvCodecUtils.h \
    NvDecoder/Utils/NvEncoderCLIOptions.h \
    fmgdecoder/ffmpegdecoder.h \
#    QmlVideo/videorender.h \
#    Videowidget/klvideowidget.h \
#    Videowidget/videodatacache.h \
#    utils/klvideorender.h \
#    utils/rendermanager.h \
#    utils/videodata.h \
#    utils/nv12render.h \
#    utils/yuvrender.h

win32{
contains(QT_ARCH, i386):{
    INCLUDEPATH += $$PWD/3rd/ffmpeg/Win32/include \
                   $$(CUDA_PATH)/include

    LIBS += $$PWD/3rd/ffmpeg/Win32/lib/avcodec.lib \
            $$PWD/3rd/ffmpeg/Win32/lib/avformat.lib \
            $$PWD/3rd/ffmpeg/Win32/lib/avutil.lib \
            $$PWD/3rd/ffmpeg/Win32/lib/swresample.lib \
            $$PWD/3rd/ffmpeg/Win32/lib/swscale.lib \
            $$(CUDA_PATH)/lib/Win32/cuda.lib \
            $$PWD/NvDecoder/Lib/Win32/nvcuvid.lib
    CONFIG(debug,release|debug):DESTDIR = $$PWD/debug/Win32
    CONFIG(release,release|debug):DESTDIR = $$PWD/debug/Win32
}else{
    INCLUDEPATH += $$PWD/3rd/ffmpeg/x64/include \
                   $$(CUDA_PATH)/include

    LIBS += $$PWD/3rd/ffmpeg/x64/lib/avcodec.lib \
            $$PWD/3rd/ffmpeg/x64/lib/avformat.lib \
            $$PWD/3rd/ffmpeg/x64/lib/avutil.lib \
            $$PWD/3rd/ffmpeg/x64/lib/swresample.lib \
            $$PWD/3rd/ffmpeg/x64/lib/swscale.lib \
            $$(CUDA_PATH)/lib/x64/cuda.lib \
            $$PWD/NvDecoder/Lib/x64/nvcuvid.lib
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
