#ifndef NVIDIADECODER_H
#define NVIDIADECODER_H

#include "../decoder.h"
#include "NvDecoder.h"
#include "Utils/FFmpegDemuxer.h"
#include <vector>
#ifndef __unix
class KLVIDEODECODSHARED_EXPORT NvidiaDecoder : public Decoder
#else
class NvidiaDecoder : public Decoder
#endif
{
public:
    ~NvidiaDecoder();
    bool initsize() override;
    unsigned char* framePtr() override;
    bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat, unsigned char *, int, int)> frameHandler, std::mutex *mtx = nullptr) override;
    void stop() override;

private:
    bool m_isRun = true;
    NvDecoder *m_nvdecod{nullptr};
    static std::vector<std::pair<CUcontext,std::string>> m_ctxV;
    unsigned char *m_ptr{nullptr};
    int m_curIndex = 0;
};

#endif // NVIDIADECODER_H
