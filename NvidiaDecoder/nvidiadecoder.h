#ifndef NVIDIADECODER_H
#define NVIDIADECODER_H

#include <vector>
#include "decoder.h"
#include "NvDecoder.h"
#include "Utils/FFmpegDemuxer.h"
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
}
class NVDECODERSHARED_EXPORT NvidiaDecoder : public Decoder
{
public:
    NvidiaDecoder() = default;
    ~NvidiaDecoder();
    bool initsize() override;
    unsigned char* framePtr() override;
    int fps() const;
    bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat, unsigned char *, int, int)> frameHandler, std::mutex *mtx = nullptr) override;
    void stop() override;

private:
    bool m_isRun = true;
    int m_fps = 0;
    NvDecoder *m_nvdecod{nullptr};
    FFmpegDemuxer *m_demuxer{nullptr};
    unsigned char *m_ptr{nullptr};
};
#endif // NVIDIADECODER_H
