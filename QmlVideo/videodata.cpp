#include "videodata.h"
#include "decoder.h"
#include <iostream>
#include <QDebug>

VideoData::VideoData(QObject *parent):
    QThread(parent)
{

}

void VideoData::setVideoSource(QString s)
{
    m_url = s;
    m_isRtsp = m_url.contains("rtsp");
}

void VideoData::setDecodeName(QString s)
{
    m_decoderName = s;
}

QString VideoData::decoder()
{
    return m_decoderName;
}

void VideoData::run()
{
    std::ostream::fmtflags oldf = std::cout.flags();
    std::mutex mtx;
    std::string errorStr;
    Decoder *decoder{nullptr};
    if(m_decoderName == "h264_cuvid"){
        decoder = Klvideodecod::createDecoder<NvidiaDecoder>();
    }else{
        decoder = Klvideodecod::createDecoder<FFmpegDecoder>();
    }

    bool isOk = decoder->decode(m_url.toStdString().data(),errorStr,[this,&decoder](AVPixelFormat format, unsigned char * ptr, int width, int height)->void{
            if(m_isFirst){
                emit sigVideoStarted(format,ptr,width,height,nullptr);
                m_isFirst = false;
            }
            if(isInterruptionRequested()){
                decoder->stop();
            }
            emit sigFrameLoaded();
    },&mtx);
    std::cout.flags(oldf);

    if(!isOk){
        emit sigError(QString::fromStdString(errorStr));
    }
    emit sigVideoStoped();
    m_isFirst = true;
    delete decoder;
}
