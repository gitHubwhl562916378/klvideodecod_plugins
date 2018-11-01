#include "videodatacache.h"
#include <QDebug>

VideoDataCache *VideoDataCache::m_Instance = new VideoDataCache;
QMap<QString,int> *VideoDataCache::m_videoPlayCountMap = new QMap<QString,int>();
QMap<QString,VideoData*> *VideoDataCache::m_videoDataMap = new QMap<QString,VideoData*>();
VideoDataCache::~VideoDataCache()
{
    for(QMap<QString,VideoData*>::Iterator iter = m_videoDataMap->begin(); iter != m_videoDataMap->end(); iter++){
        delete iter.value();
    }
}

VideoData *VideoDataCache::createVideoData(QString url, QString decoderName)
{
    VideoData *decoThr = m_videoDataMap->value(url);
    if(!decoThr){
        decoThr = new VideoData;
        decoThr->setDecoderName(decoderName);
        decoThr->setVideoSource(url);
    }
    m_videoDataMap->insert(url,decoThr);
    (*m_videoPlayCountMap)[url]++;
    qDebug() << "videodata added" << url << m_videoPlayCountMap->value(url);

    return decoThr;
}

void VideoDataCache::removeVideoData(QString url)
{
    if(!m_videoPlayCountMap->contains(url))
    {
        qDebug()  << "no such video data" << url;
        return;
    }
    (*m_videoPlayCountMap)[url]--;
    qDebug() << "videodata removed" << url << m_videoPlayCountMap->value(url);
    if(!m_videoPlayCountMap->value(url)){
        VideoData* decoTrh = m_videoDataMap->take(url);
        if(decoTrh->isRunning()){
            connect(decoTrh,SIGNAL(finished()),decoTrh,SLOT(deleteLater()));
            decoTrh->requestInterruption();
        }else{
            delete decoTrh;
        }
        m_videoPlayCountMap->take(url);
    }
}
