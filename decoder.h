#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <functional>
#include <mutex>
#include "klvideodecod_global.h"
extern "C"
{
    #include <libavutil/pixfmt.h>
}

class KLVIDEODECODSHARED_EXPORT Decoder
{
public:
    explicit Decoder() = default;
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder &) = delete;
    virtual ~Decoder(){}
    virtual unsigned char* framePtr() = 0;
    virtual bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx = nullptr) = 0;
    virtual void stop() = 0;
};

#include "NvDecoder/nvidiadecoder.h"
#include "fmgdecoder/ffmpegdecoder.h"
class KLVIDEODECODSHARED_EXPORT Klvideodecod
{
public:
    explicit Klvideodecod() = default;
    Klvideodecod(const Klvideodecod&) = delete;
    Klvideodecod& operator=(const Klvideodecod&) = delete;
    virtual ~Klvideodecod(){}
    template<class T> static Decoder* createDecoder(){return new T;}
};
#endif // DECODER_H
