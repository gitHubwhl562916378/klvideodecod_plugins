#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <QThread>
#include <QMutex>
#include <mutex>
#include <QTimer>
#include "klvideowidget_global.h"
#include "decoder.h"
class KLVIDEOWIDGETSHARED_EXPORT VideoData : public QThread
{
    Q_OBJECT
public:
    int m_width,m_height;
    int m_fmt;
    unsigned char *m_ptr{nullptr};
    std::mutex *m_mtx{nullptr};

    VideoData(QObject *parent = nullptr);
    ~VideoData();
    void setVideoSource(QString);
    bool isDecoding() const;
    unsigned char* framePtr();
    QString videoSource() const;
    void setDecoderName(QString);
    QString decoder();
    QString errorStr();

signals:
    void sigError(QString);
    void sigVideoStarted();
    void sigFrameLoaded();

protected:
    void run() override;

private:
    QString m_url,m_decoderName,m_errorStr;
    bool m_isRtsp = false;
    bool m_isFirst = true;
    bool m_isDecoding = false;
    Decoder *m_decoder{nullptr};

    Decoder* loadPlugin(QString);
};

#endif // VIDEODATA_H
