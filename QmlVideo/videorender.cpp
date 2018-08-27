#include "videorender.h"
#include "../utils/rendermanager.h"
#include "../utils/videodata.h"
#include <memory>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
}
#include <QDebug>

VideoRender::VideoRender(QQuickItem *parent):
    QQuickFramebufferObject(parent)
{
}

VideoRender::~VideoRender()
{
    stop();
    if(m_videoData){
        m_videoData->quit();
        m_videoData->wait();
    }
    if(m_rgba){
        delete m_rgba;
    }
}

void VideoRender::play()
{
    if(m_videoData){
        stop();
    }

    m_videoData = new VideoData(this);
    connect(m_videoData,&VideoData::sigError,this,[this](QString s){
        m_ptr = nullptr;
        m_width = 0;
        m_height = 0;
        m_isplaying = false;
        update();
        setError(s);});
    connect(m_videoData,&VideoData::finished,this,[this]{
        m_ptr = nullptr;
        m_width = 0;
        m_height = 0;
        m_isplaying = false;
        update();
        emit videoStoped();
    });
    connect(m_videoData,&VideoData::sigVideoStarted,this,[this]{
        qDebug() << "video started";
        m_format = AVPixelFormat(m_videoData->m_fmt);
        m_ptr = m_videoData->m_ptr;
        m_width = m_videoData->m_width;
        m_height = m_videoData->m_height;
        m_mtx = m_videoData->m_mtx;
        m_isplaying = true;
        if(m_rgba){
            delete m_rgba;
        }
        m_rgba = new unsigned char[4 * m_width * m_height];
        emit videoStarted();
        update();
    });
    connect(m_videoData,&VideoData::sigFrameLoaded,this,[this]{
        if(m_videoData){
            m_ptr = m_videoData->framePtr();
        }else{
            m_ptr = nullptr;
        }
        update();
    });
    connect(m_videoData,&VideoData::finished,this,[this]{m_videoData->deleteLater();m_videoData = nullptr;});

    m_videoData->setVideoSource(m_videoSource);
    m_videoData->setDecoderName(m_decoderName);
    m_videoData->start();
}

void VideoRender::stop()
{
    if(m_videoData){
        m_videoData->disconnect(this);
        m_videoData->requestInterruption();
    }
}

bool VideoRender::isPlaying()
{
    return m_isplaying;
}

QImage VideoRender::frame()
{
    if(!m_ptr){
        return QImage();
    }

    AVPicture pFrameI,pFrameBGR;
    QSize s = boundingRect().size().toSize();
    avpicture_fill(&pFrameI,m_ptr, m_format,m_width,m_height);
    avpicture_fill(&pFrameBGR,m_rgba,AV_PIX_FMT_RGB32,s.width(),s.height());

    SwsContext* imgCtx = NULL;
    imgCtx = sws_getContext(m_width,m_height,m_format,s.width(),s.height(),AV_PIX_FMT_RGB32,SWS_BILINEAR,0,0,0);

    if (imgCtx != NULL){
        if(m_mtx) m_mtx->lock();
        sws_scale(imgCtx,pFrameI.data,pFrameI.linesize,0,m_height,pFrameBGR.data,pFrameBGR.linesize);
        if(m_mtx) m_mtx->unlock();
        if(imgCtx){
            sws_freeContext(imgCtx);
            imgCtx = NULL;
        }
    }else{
        sws_freeContext(imgCtx);
        imgCtx = NULL;
    }

    return QImage(m_rgba,s.width(), s.height(),QImage::Format_RGB32);
}

QQuickFramebufferObject::Renderer *VideoRender::createRenderer() const
{
    return new VideoFboRenderer;
}

QString VideoRender::videoSource()
{
    return m_videoSource;
}

void VideoRender::setVideoSource(QString s)
{
    if(s != m_videoSource){
        emit videoSourceChanged();
        m_videoSource = s;
    }
}

QString VideoRender::error()
{
    return m_errorStr;
}

void VideoRender::setError(QString s)
{
    m_errorStr = s;
    emit errorOcured();
}

QString VideoRender::decoder()
{
    return m_decoderName;
}

void VideoRender::setDecoder(QString d)
{
    m_decoderName = d;
}

VideoFboRenderer::VideoFboRenderer()
{
    m_renderM = new RenderManager;
}

VideoFboRenderer::~VideoFboRenderer()
{
    delete m_renderM;
}

void VideoFboRenderer::synchronize(QQuickFramebufferObject *item)
{
    VideoRender *m_videoR = dynamic_cast<VideoRender*>(item);
    m_format = m_videoR->m_format;
    m_ptr = m_videoR->m_ptr;
    m_width = m_videoR->m_width;
    m_height = m_videoR->m_height;
    m_mtx = m_videoR->m_mtx;
}

void VideoFboRenderer::render()
{
    if(!m_renderM->hasRender(m_format)){
        m_renderM->registerRender(m_format,false,true);
    }

    if(m_mtx) m_mtx->lock();
    m_renderM->render(m_format,m_ptr,m_width,m_height);
    if(m_mtx) m_mtx->unlock();
}
