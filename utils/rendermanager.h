#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "klvideorender.h"
#include <map>

class RenderManager : public KLvideoRenderManager
{
public:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    ~RenderManager();
    bool registerRender(AVPixelFormat,bool horizontal = false, bool vertical = false) override;
    bool hasRender(AVPixelFormat) override;
    void render(AVPixelFormat,unsigned char*,int,int) override;

private:
    std::map<AVPixelFormat,KLvideoRender*> m_rendermap;
};

#endif // RENDERMANAGER_H
