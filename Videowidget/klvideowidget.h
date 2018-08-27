#ifndef KLVIDEOWIDGET_H
#define KLVIDEOWIDGET_H

#include "klvideodecod_global.h"
#include <QOpenGLWidget>
#include "../utils/klvideorender.h"
#include <mutex>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(VideoData)
class KLVIDEODECODSHARED_EXPORT Klvideowidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    enum PlayState{
        Stop,
        Reading,
        Playing
    };
    Klvideowidget(QWidget *parent = nullptr);
    ~Klvideowidget();
    PlayState playState() const;
    void startPlay(QString url,QString decoderName);
    int videoWidth() const;
    int videoHeidht() const;
    QString url() const;
    QString decoderName() const;

public slots:
    void stop();

signals:
    void sigError(QString);
    void sigVideoStart(int,int);
    void sigVideoStoped();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    PlayState m_state = Stop;
    std::mutex *m_mtx{nullptr};
    KLvideoRenderManager *m_renderM{nullptr};
    uchar* m_ptr{nullptr};
    AVPixelFormat m_fmt;
    int m_videoW,m_videoH;
    VideoData *m_decoThr{nullptr};
    QString m_url,m_decoderName;

private slots:
    void slotVideoStarted();
};

#endif // KLVIDEOWIDGET_H
