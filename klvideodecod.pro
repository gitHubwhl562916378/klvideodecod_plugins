#-------------------------------------------------
#
# Project created by QtCreator 2018-08-17T10:56:11
#
#-------------------------------------------------

QT       -= gui
#QT       += quick
#CONFIG -= qt

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
#    QmlVideo/nv12render.cpp \
#    QmlVideo/rendermanager.cpp \
#    QmlVideo/videodata.cpp \
#    QmlVideo/videorender.cpp \
#    QmlVideo/yuvrender.cpp

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
#    QmlVideo/klvideorender.h \
#    QmlVideo/nv12render.h \
#    QmlVideo/rendermanager.h \
#    QmlVideo/videodata.h \
#    QmlVideo/videorender.h \
#    QmlVideo/yuvrender.h

win32{
contains(QT_ARCH, i386):{
}else{
    INCLUDEPATH += D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/include \
                   $$(CUDA_PATH)/include

    LIBS += D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/lib/avcodec.lib \
            D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/lib/avformat.lib \
            D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/lib/avutil.lib \
            D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/lib/swresample.lib \
            D:/git/KoalaClient/klvideowidget/klvideodata/3rd/ffmpeg/x64/lib/swscale.lib \
            $$(CUDA_PATH)/lib/x64/cuda.lib \
            $$PWD/NvDecoder/Lib/x64/nvcuvid.lib
}
}

unix {
INCLUDEPATH += /usr/local/cuda/include \
               /usr/local/ffmpeg/include
LIBS += -L/usr/local/ffmpeg/lib -lnvcuvid -lcuda \
        -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
    target.path = /usr/lib
    INSTALLS += target
}
