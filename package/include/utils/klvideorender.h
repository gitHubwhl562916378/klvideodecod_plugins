#ifndef KLVIDEORENDER_H
#define KLVIDEORENDER_H

extern "C" {
#include "libavutil/pixfmt.h"
}
#include "klvideowidget_global.h"
class KLVIDEOWIDGETSHARED_EXPORT KLvideoRender
{
public:
    virtual ~KLvideoRender(){}
    virtual void initialize(bool horizontal = false, bool vertical = false) = 0;
    virtual void render(unsigned char*,int,int) = 0;
};

class KLVIDEOWIDGETSHARED_EXPORT KLvideoRenderManager
{
public:
   virtual bool registerRender(AVPixelFormat,bool horizontal = false, bool vertical = false) = 0;
   virtual bool hasRender(AVPixelFormat) = 0;
   virtual void render(AVPixelFormat,unsigned char*,int,int) = 0;
   virtual ~KLvideoRenderManager(){}
};
#endif // KLVIDEORENDER_H
