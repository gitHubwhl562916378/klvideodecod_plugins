#include "videodata.h"
#include <iostream>
#include <QDebug>
#include <QLibrary>

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
        m_decoder = loadPlugin("decoderPlugins/NvidiaDecoder");
        if(!m_decoder){
            qInfo() << "load " + m_decoderName + " failed,try FmgDecoder";
            m_decoder = loadPlugin("decoderPlugins/FmgDecoder");
            if(!m_decoder){
                qInfo() << "load FmgDecoder failed";
                return;
            }
        }else{
            if(!m_decoder->initsize()){
                delete m_decoder;
                qInfo() << "NvidiaDecoder initsize failed,try FmgDecoder";
                m_decoder = loadPlugin("decoderPlugins/FmgDecoder");
                if(!m_decoder){
                    qInfo() << "load FmgDecoder failed";
                    return;
                }
            }
        }
    }else{
        m_decoder = loadPlugin("decoderPlugins/FmgDecoder");
        if(!m_decoder){
            errorStr.assign("load fmg_decoder failed");
            return;
        }
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

Decoder *VideoData::loadPlugin(QString fileName)
{
    typedef Decoder* (*CreatePlugin)();
    QLibrary dllLoad(fileName);
    if(dllLoad.load()){
        CreatePlugin createFunc = (CreatePlugin)dllLoad.resolve("createDecoder");
        if(createFunc){
            return createFunc();
        }
    }else{
        qDebug() << dllLoad.errorString();
        return nullptr;
    }
}
