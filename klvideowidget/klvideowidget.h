#ifndef KLVIDEOWIDGET_H
#define KLVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <mutex>
#include <QTimer>
#include "klvideowidget_global.h"
#include "utils/klvideorender.h"

QT_FORWARD_DECLARE_CLASS(VideoData)
class KLRender{
public:
    virtual void initsize(QOpenGLContext *ctx) = 0;
    virtual void render(QOpenGLContext *ctx) = 0;
};

class KLVIDEOWIDGETSHARED_EXPORT Klvideowidget : public QOpenGLWidget
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
    virtual KLRender* createRender() const{return nullptr;}
    void initializeGL() override;
    void paintGL() override;

private:
    PlayState m_state = Stop;
    KLvideoRenderManager *m_renderM{nullptr};
    AVPixelFormat m_fmt;
    int m_videoW,m_videoH;
    VideoData *m_decoThr{nullptr};
    QString m_url,m_decoderName;
    KLRender *render_{nullptr};

private slots:
    void slotVideoStarted();
};

#endif // KLVIDEOWIDGET_H
