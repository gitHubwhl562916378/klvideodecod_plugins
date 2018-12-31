#ifndef FMGDECODER_H
#define FMGDECODER_H

#include "decoder.h"
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
}
class FMGDECODERSHARED_EXPORT Fmgdecoder : public Decoder
{
public:
    Fmgdecoder() = default;
    Fmgdecoder(const Fmgdecoder&) = delete;
    Fmgdecoder& operator=(const Fmgdecoder &) = delete;
    ~Fmgdecoder();
    bool initsize() override;
    unsigned char* framePtr() override;
    int fps() const;
    bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler,std::mutex *mtx=nullptr) override;
    void stop() override;

private:
    bool m_run = true;
    int m_fps = 0;
    unsigned char *m_buffer{nullptr};
    std::mutex *m_mtx{nullptr};
};
#endif // FMGDECODER_H
