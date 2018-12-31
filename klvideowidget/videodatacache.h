#ifndef VIDEODATACACHE_H
#define VIDEODATACACHE_H

#include "utils/videodata.h"
#include <QMap>
class VideoDataCache : QObject
{
    Q_OBJECT
public:
    ~VideoDataCache();
    static VideoData* createVideoData(QString url , QString decodFactoryName);
    static void removeVideoData(QString);

private:
    static VideoDataCache *m_Instance;
    static QMap<QString,int> *m_videoPlayCountMap;
    static QMap<QString,VideoData*> *m_videoDataMap;
};

#endif // VIDEODATACACHE_H
