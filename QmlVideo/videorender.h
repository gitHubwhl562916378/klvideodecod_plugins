#ifndef VIDEORENDER_H
#define VIDEORENDER_H
#include "klvideorender.h"
#include <QQuickFramebufferObject>
#include <QImage>
#include <QMutex>

class VideoData;
class KLVIDEODECODSHARED_EXPORT VideoFboRenderer : public QQuickFramebufferObject::Renderer
{
public:
    VideoFboRenderer();
    ~VideoFboRenderer();
    void synchronize(QQuickFramebufferObject*);
    void render() override;

private:
    KLvideoRenderManager *m_renderM{nullptr};
    QMutex *m_mtx{nullptr};
    AVPixelFormat m_format;
    unsigned char* m_ptr{nullptr};
    int m_width = 0,m_height = 0;
};

class VideoRender : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString videoSource READ videoSource WRITE setVideoSource NOTIFY videoSourceChanged)
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorOcured)
    Q_PROPERTY(QString decoder READ decoder WRITE setDecoder NOTIFY decoderChanged)
public:
    QMutex *m_mtx{nullptr};
    AVPixelFormat m_format;
    unsigned char* m_ptr{nullptr};
    int m_width = 0,m_height = 0;

    explicit VideoRender(QQuickItem* parent = nullptr);
    ~VideoRender();
    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isPlaying();
    Q_INVOKABLE QImage frame();
    Renderer* createRenderer() const override;

signals:
    void errorOcured();
    void videoSourceChanged();
    void videoStoped();
    void videoStarted();
    void videoState(bool state);
    void decoderChanged();

private:
    QString m_videoSource,m_errorStr;
    bool m_isplaying = false;
    VideoData *m_videoData{nullptr};
    unsigned char *m_rgba{nullptr};

    QString videoSource();
    void setVideoSource(QString);
    QString error();
    void setError(QString);
    QString decoder();
    void setDecoder(QString);
};

#endif // VIDEORENDER_H
