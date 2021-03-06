#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <functional>
#include <mutex>
#include "nvdecoder_global.h"
extern "C"
{
    #include <libavutil/pixfmt.h>
}

class NVDECODERSHARED_EXPORT Decoder
{
public:
    explicit Decoder() = default;
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder &) = delete;
    virtual ~Decoder(){}
    virtual bool initsize() = 0;
    virtual unsigned char* framePtr() = 0;
    virtual int fps() const = 0;
    virtual bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx = nullptr) = 0;
    virtual void stop() = 0;
};

extern "C"
{
    NVDECODERSHARED_EXPORT Decoder* createDecoder();
}
#endif // DECODER_H
