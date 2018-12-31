#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "klvideowidget.h"
#include "utils/rendermanager.h"
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
    makeCurrent();
    delete m_renderM;
    if(render_){
        delete render_;
    }
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
    connect(m_decoThr,SIGNAL(finished()),this,SLOT(stop()));
    connect(m_decoThr,&VideoData::destroyed,this,[this](QObject*obj){
        if(qobject_cast<VideoData*>(obj) == m_decoThr){
            m_decoThr = nullptr;
            qDebug() << "video data initsized------------";
        }
        qDebug() << "video data prepare removed";
    },Qt::DirectConnection);
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
        disconnect(m_decoThr,SIGNAL(sigVideoStarted()),this,SLOT(slotVideoStarted()));
        disconnect(m_decoThr,SIGNAL(sigFrameLoaded()),this,SLOT(update()));
        disconnect(m_decoThr,SIGNAL(finished()),this,SLOT(stop()));
        disconnect(m_decoThr,SIGNAL(sigError(QString)),this,SIGNAL(sigError(QString)));
        m_decoThr = nullptr;
    }
    update();
    emit sigVideoStoped();
    VideoDataCache::removeVideoData(url());
}

void Klvideowidget::initializeGL()
{
    m_renderM->registerRender(AV_PIX_FMT_NV12);
    m_renderM->registerRender(AV_PIX_FMT_YUV420P);
    render_ = createRender();
    if(render_){
        render_->initsize(QOpenGLContext::currentContext());
    }
}

void Klvideowidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!m_decoThr)return;
    if(m_decoThr->m_mtx){
        m_decoThr->m_mtx->lock();
        m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),m_decoThr->framePtr(),m_videoW,m_videoH);
        m_decoThr->m_mtx->unlock();
    }else{
        m_renderM->render(AVPixelFormat(m_decoThr->m_fmt),m_decoThr->framePtr(),m_videoW,m_videoH);
    }
    if(render_){
        render_->render(QOpenGLContext::currentContext());
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
