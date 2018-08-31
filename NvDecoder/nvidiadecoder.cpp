#include "nvidiadecoder.h"

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();
std::vector<std::pair<CUcontext,std::string>> NvidiaDecoder::m_ctxV;
NvidiaDecoder::~NvidiaDecoder()
{
//    int n = m_ctxV.size();
//    for(int i = 0; i < n; i++){
//        cuCtxDestroy(m_ctxV.back().first);
//        m_ctxV.pop_back();
//    }
    if(m_nvdecod)
    delete m_nvdecod;
    if(m_demuxer)
    delete m_demuxer;
}

bool NvidiaDecoder::initsize()
{
    static bool isInitsized = false;
    if(!isInitsized){
        ck(cuInit(0));
        int nGpu = 0;
        ck(cuDeviceGetCount(&nGpu));
        for(int i = 0; i < nGpu; i++){
            CUdevice cuDevice = 0;
            ck(cuDeviceGet(&cuDevice, i));
            char szDeviceName[80];
            ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
            CUcontext cuContext = NULL;
            ck(cuCtxCreate(&cuContext, CU_CTX_SCHED_BLOCKING_SYNC, cuDevice));
            LOG(INFO) << "Find Gpu: " << szDeviceName << std::endl;

            CUVIDDECODECAPS videoDecodeCaps = {};
            videoDecodeCaps.eCodecType = cudaVideoCodec_H264;
            videoDecodeCaps.eChromaFormat = cudaVideoChromaFormat_420;
            videoDecodeCaps.nBitDepthMinus8 = 0;
            CUresult resCode;
            if ((resCode = cuvidGetDecoderCaps(&videoDecodeCaps)) == CUDA_SUCCESS){
                LOG(INFO) << "cuvid Decoder Caps nMaxWidth " << videoDecodeCaps.nMaxWidth << " nMaxHeigth " << videoDecodeCaps.nMaxHeight << std::endl;
                if(videoDecodeCaps.nMaxWidth >= 1920 && videoDecodeCaps.nMaxHeight >= 1080){
                    m_ctxV.push_back({cuContext,szDeviceName});
                }
            }else{
                LOG(INFO) << "cuvidGetDecoderCaps failed, Code: " << resCode << std::endl;
            }
        }
        isInitsized = true;
    }

    if(m_ctxV.empty()){
        return false;
    }

    return true;
}

unsigned char *NvidiaDecoder::framePtr()
{
    return m_ptr;
}

int NvidiaDecoder::fps() const
{
    return m_fps;
}

bool NvidiaDecoder::decode(const char *source, std::string &erroStr, std::function<void(AVPixelFormat, unsigned char *, int, int)> frameHandler, std::mutex *mtx)
{
    if(!m_ctxV.size()){
        return false;
    }

    try{
        m_isRun = true;
        std::pair<CUcontext,std::string> v = m_ctxV.at(m_curIndex++ % m_ctxV.size());
        std::cout << "Use Contex in " << v.second << std::endl;
        m_demuxer = new FFmpegDemuxer(source);
        m_fps = m_demuxer->fps();
        m_nvdecod = new NvDecoder(v.first, m_demuxer->GetWidth(), m_demuxer->GetHeight(), false, FFmpeg2NvCodecId(m_demuxer->GetVideoCodec()),mtx);
        int nVideoBytes = 0, nFrameReturned = 0, nFrame = 0;
        uint8_t *pVideo = NULL;
        uint8_t **ppFrame;
        do {
            m_demuxer->Demux(&pVideo, &nVideoBytes);
            m_nvdecod->Decode(pVideo, nVideoBytes, &ppFrame, &nFrameReturned);
            if (!nFrame && nFrameReturned)
                LOG(INFO) << m_nvdecod->GetVideoInfo();

            for (int i = 0; i < nFrameReturned; i++) {
                if (m_nvdecod->GetBitDepth() == 8){
                    m_ptr = ppFrame[i];
                    frameHandler(AV_PIX_FMT_NV12,m_ptr,m_nvdecod->GetWidth(),m_nvdecod->GetHeight());
                }else{
//                    P016ToBgra32((uint8_t *)ppFrame[i], 2 * dec.GetWidth(), (uint8_t *)dpFrame, nPitch, dec.GetWidth(), dec.GetHeight());
                }
            }
            nFrame += nFrameReturned;
        } while (nVideoBytes && m_isRun);
        std::cout << "Total frame decoded: " << nFrame << std::endl;
        if(m_isRun){
            if(!nVideoBytes){
                erroStr = "AVERROR_EOF";
                return false;
            }
        }

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
