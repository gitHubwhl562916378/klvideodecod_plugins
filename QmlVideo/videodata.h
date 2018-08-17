#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <QThread>
#include <QMutex>
#include "../klvideodecod_global.h"
class KLVIDEODECODSHARED_EXPORT VideoData : public QThread
{
    Q_OBJECT
public:
    VideoData(QObject *parent = nullptr);
    void setVideoSource(QString);
    void setDecodeName(QString);
    QString decoder();

signals:
    void sigError(QString);
    void sigVideoStarted(int,unsigned char*,int,int,QMutex*);
    void sigVideoStoped();
    void sigFrameLoaded();

protected:
    void run() override;

private:
    QString m_url,m_decoderName;
    bool m_isRtsp = false;
    bool m_isFirst = true;
};

#endif // VIDEODATA_H
