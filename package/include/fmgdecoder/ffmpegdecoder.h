#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "../decoder.h"
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
}
#ifndef __unix
class KLVIDEODECODSHARED_EXPORT FFmpegDecoder : public Decoder
#else
class FFmpegDecoder : public Decoder
#endif
{
public:
    ~FFmpegDecoder();
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

#endif // FFMPEGDECODER_H
