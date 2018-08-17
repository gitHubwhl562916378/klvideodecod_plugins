#include "videorender.h"
#include "rendermanager.h"
#include "videodata.h"
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
    m_videoData = new VideoData(this);
    connect(m_videoData,&VideoData::sigError,this,[this](QString s){
        m_ptr = nullptr;
        m_width = 0;
        m_height = 0;
        m_isplaying = false;
        update();
        setError(s);});
    connect(m_videoData,&VideoData::sigVideoStoped,this,[this]{
        m_ptr = nullptr;
        m_width = 0;
        m_height = 0;
        m_isplaying = false;
        update();
        emit videoStoped();
    });
    connect(m_videoData,&VideoData::sigVideoStarted,this,[this](int format,unsigned char * ptr, int w, int h, QMutex *mtx){
        m_format = AVPixelFormat(format);
        m_ptr = ptr;
        m_width = w;
        m_height = h;
        m_mtx = mtx;
        m_isplaying = true;
        if(m_rgba){
            delete m_rgba;
        }
        m_rgba = new unsigned char[4 * w * h];
        emit videoStarted();
        update();
    });
    connect(m_videoData,SIGNAL(sigFrameLoaded()),this,SLOT(update()));
}

VideoRender::~VideoRender()
{
    stop();
    if(m_rgba){
        delete m_rgba;
    }
}

void VideoRender::play()
{
    if(m_videoData->isRunning()){
        return;
    }

    m_videoData->setVideoSource(m_videoSource);
    m_videoData->start();
}

void VideoRender::stop()
{
    m_videoData->requestInterruption();
    m_videoData->quit();
    m_videoData->wait();
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
    return m_videoData->decoder();
}

void VideoRender::setDecoder(QString d)
{
    m_videoData->setDecodeName(d);
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
        m_renderM->registerRender(m_format);
    }

    if(m_mtx) m_mtx->lock();
    m_renderM->render(m_format,m_ptr,m_width,m_height);
    if(m_mtx) m_mtx->unlock();
}
