#include "nvidiadecoder.h"

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();
unsigned char *NvidiaDecoder::framePtr()
{
    return nullptr;
}

bool NvidiaDecoder::decode(const char *source, std::string &erroStr, std::function<void(AVPixelFormat, unsigned char *, int, int)> frameHandler, std::mutex *mtx)
{
    try{
        m_isRun = true;
        FFmpegDemuxer demuxer(source);
        NvDecoder dec(getContext(), demuxer.GetWidth(), demuxer.GetHeight(), false, FFmpeg2NvCodecId(demuxer.GetVideoCodec()),mtx);
        int nVideoBytes = 0, nFrameReturned = 0, nFrame = 0;
        uint8_t *pVideo = NULL;
        uint8_t **ppFrame;
        do {
            demuxer.Demux(&pVideo, &nVideoBytes);
            dec.Decode(pVideo, nVideoBytes, &ppFrame, &nFrameReturned);
            if (!nFrame && nFrameReturned)
                LOG(INFO) << dec.GetVideoInfo();

            for (int i = 0; i < nFrameReturned; i++) {
                if (dec.GetBitDepth() == 8){
                    frameHandler(AV_PIX_FMT_NV12,ppFrame[i],dec.GetWidth(),dec.GetHeight());
                }else{
//                    P016ToBgra32((uint8_t *)ppFrame[i], 2 * dec.GetWidth(), (uint8_t *)dpFrame, nPitch, dec.GetWidth(), dec.GetHeight());
                }
            }
            nFrame += nFrameReturned;
        } while (nVideoBytes && m_isRun);
        std::cout << "Total frame decoded: " << nFrame << std::endl;
        return true;
    }catch(std::exception &e){
        erroStr = e.what();
        return false;
    }
}

void NvidiaDecoder::stop()
{
    m_isRun = false;
}

CUcontext NvidiaDecoder::getContext()
{
    return ContextManager::pInstance()->getContext();
}
