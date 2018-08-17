#ifndef YUVRENDER_H
#define YUVRENDER_H
#include "klvideorender.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class KLVIDEODECODSHARED_EXPORT YuvRender : public QOpenGLFunctions , public KLvideoRender
{
public:
    YuvRender() = default;
    YuvRender(const YuvRender &) = delete;
    ~YuvRender();
    void initialize() override;
    void render(uchar*yuvPtr, int w, int h) override;

private:
    QOpenGLShaderProgram program;
    GLuint idY,idU,idV,textures[2];
    QOpenGLBuffer vbo;
};

#endif // YUVRENDER_H
