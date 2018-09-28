#include "videodata.h"
#include <iostream>
#include <QDebug>

VideoData::VideoData(QObject *parent):
    QThread(parent)
{
//    m_mtx = new std::mutex;
}

VideoData::~VideoData()
{
    if(m_mtx){
        delete m_mtx;
    }
    if(m_decoder) delete m_decoder;
    m_decoder = nullptr;
    qDebug() << "video" << m_url << "deleted";
}

void VideoData::setVideoSource(QString s)
{
    m_url = s;
    m_isRtsp = m_url.contains("rtsp");
}

bool VideoData::isDecoding() const
{
    return m_isDecoding;
}

unsigned char *VideoData::framePtr()
{
    if(!m_decoder)return nullptr;
    return m_decoder->framePtr();
}

QString VideoData::videoSource() const
{
    return m_url;
}

void VideoData::setDecoderName(QString s)
{
    m_decoderName = s;
}

QString VideoData::decoder()
{
    return m_decoderName;
}

QString VideoData::errorStr()
{
    return m_errorStr;
}

void VideoData::run()
{
    std::string errorStr;
    m_errorStr.clear();
    if(m_decoderName == "h264_cuvid"){
        m_decoder = Klvideodecod::createDecoder<NvidiaDecoder>();
        if(!m_decoder->initsize()){
            delete m_decoder;
            m_decoder = Klvideodecod::createDecoder<FFmpegDecoder>();
        }
    }else{
        m_decoder = Klvideodecod::createDecoder<FFmpegDecoder>();
    }
    bool isOk = m_decoder->decode(m_url.toStdString().data(),errorStr,[this](AVPixelFormat format,unsigned char*ptr, int width, int height)->void{
            if(m_isFirst){
                m_width = width;
                m_height = height;
                m_fmt = format;
                m_isFirst = false;
                m_isDecoding = true;
                emit sigVideoStarted();
            }
            if(isInterruptionRequested()){
                m_isDecoding = false;
                if(m_decoder){
                    m_decoder->stop();
                }
            }else{
                m_ptr = ptr;
                emit sigFrameLoaded();
                if(!m_isRtsp){
                    QThread::msleep(::floor(1000 / m_decoder->fps()));
                }
            }
    },m_mtx);
    m_isFirst = true;
    m_isDecoding = false;
    if(!isOk){
        m_errorStr = QString::fromStdString(errorStr);
        emit sigError(m_errorStr);
    }
}
