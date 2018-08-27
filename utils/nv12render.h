#ifndef NV12RENDER_H
#define NV12RENDER_H
#include "klvideorender.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Nv12Render : public QOpenGLFunctions, public KLvideoRender
{
public:
    Nv12Render() = default;
    Nv12Render(const Nv12Render&) = delete;
    ~Nv12Render();
    void initialize(bool horizontal = false, bool vertical = false) override;
    void render(uchar*nv12Ptr, int w, int h) override;

private:
    QOpenGLShaderProgram program;
    GLuint idY,idUV, textures[2];
    QOpenGLBuffer vbo;
};

#endif // NV12RENDER_H
