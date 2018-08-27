#include "klvideowidget.h"
#include "../utils/rendermanager.h"
#include "videodatacache.h"
#include <iostream>
#include <QDebug>

Klvideowidget::Klvideowidget(QWidget *parent):
    QOpenGLWidget(parent)
{
    m_renderM = new RenderManager;
}

Klvideowidget::~Klvideowidget()
{
    stop();
    delete m_renderM;
}

Klvideowidget::PlayState Klvideowidget::playState() const
{
    return m_state;
}

void Klvideowidget::startPlay(QString url, QString decoderName)
{
    m_state = Reading;
    m_decoThr = VideoDataCache::createVideoData(url,decoderName);
    m_url = url;
    m_decoderName = decoderName;
    m_mtx = m_decoThr->m_mtx;
    if(m_decoThr->isDecoding()){
        m_videoW = m_decoThr->m_width;
        m_videoH = m_decoThr->m_height;
        m_state = Playing;
        emit sigVideoStart(m_decoThr->m_width,m_decoThr->m_height);
    }else{
//        if(!m_decoThr->errorStr().isEmpty()){
//            emit sigError(m_decoThr->errorStr());
//        }
        connect(m_decoThr,SIGNAL(sigVideoStarted()),this,SLOT(slotVideoStarted()));
    }
    connect(m_decoThr,SIGNAL(sigFrameLoaded()),this,SLOT(update()));
    connect(m_decoThr,&VideoData::finished,this,[this]{m_decoThr = nullptr;stop();});
    connect(m_decoThr,SIGNAL(sigError(QString)),this,SIGNAL(sigError(QString)));
    m_decoThr->start();
}

int Klvideowidget::videoWidth() const
{
    return m_videoW;
}

int Klvideowidget::videoHeidht() const
{
    return m_videoH;
}

QString Klvideowidget::url() const
{
    return m_url;
}

QString Klvideowidget::decoderName() const
{
    return m_decoderName;
}

void Klvideowidget::stop()
{
    m_state = Stop;
    if(m_decoThr){
        m_decoThr->disconnect(this);
        m_decoThr = nullptr;
    }
    m_ptr = nullptr;
    m_mtx = nullptr;
    update();
    emit sigVideoStoped();
    VideoDataCache::removeVideoData(url());
}

void Klvideowidget::initializeGL()
{
    m_renderM->registerRender(AV_PIX_FMT_NV12);
    m_renderM->registerRender(AV_PIX_FMT_YUV420P);
}

void Klvideowidget::paintGL()
{
    if(!m_decoThr)return;
    if(m_mtx){
        m_mtx->lock();
        if(m_state != Playing){
            m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),nullptr,m_videoW,m_videoH);
        }else{
            m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),m_decoThr->framePtr(),m_videoW,m_videoH);
        }
        m_mtx->unlock();
    }else{
        if(m_state != Playing){
            m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),nullptr,m_videoW,m_videoH);
        }else{
            m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),m_decoThr->framePtr(),m_videoW,m_videoH);
        }
    }
}

void Klvideowidget::slotVideoStarted()
{
    if(!m_decoThr)return;
    m_videoW = m_decoThr->m_width;
    m_videoH = m_decoThr->m_height;
    m_state = Playing;
    emit sigVideoStart(m_decoThr->m_width,m_decoThr->m_height);
}
