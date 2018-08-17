#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "../decoder.h"

class KLVIDEODECODSHARED_EXPORT FFmpegDecoder : public Decoder
{
public:
    bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx = nullptr) override;
    void stop() override;

private:
    bool m_run = true;
};

#endif // FFMPEGDECODER_H
