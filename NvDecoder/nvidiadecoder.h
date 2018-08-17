#ifndef NVIDIADECODER_H
#define NVIDIADECODER_H

#include "../decoder.h"
#include "NvDecoder.h"
#include "Utils/FFmpegDemuxer.h"
#include <vector>
class KLVIDEODECODSHARED_EXPORT NvidiaDecoder : public Decoder
{
public:
    bool decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx = nullptr) override;
    void stop() override;

private:
    bool m_isRun = true;
    class ContextManager
    {
    public:
        static ContextManager* pInstance(){
            static ContextManager ctxM;
            return &ctxM;
        }
        CUcontext getContext(){
            if(!m_ctxV.size()){
                return nullptr;
            }

            std::pair<CUcontext,std::string> &v = m_ctxV.at(m_curIndex++ % m_ctxV.size());
            std::cout << "Use Contex in " << v.second << std::endl;
            return v.first;
        }
        ~ContextManager(){
            int n = m_ctxV.size();
            for(int i = 0; i < n; i++){
                cuCtxDestroy(m_ctxV.back().first);
                m_ctxV.pop_back();
            }
        }
    private:
        std::vector<std::pair<CUcontext,std::string>> m_ctxV;
        int m_curIndex = 0;

        explicit ContextManager(){
            ck(cuInit(0));
            int nGpu = 0;
            ck(cuDeviceGetCount(&nGpu));
            for(int i = 0; i < nGpu; i++){
                CUdevice cuDevice = 0;
                ck(cuDeviceGet(&cuDevice, i));
                char szDeviceName[80];
                ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
                std::cout << "Find Gpu: " << szDeviceName << std::endl;
                CUcontext cuContext = NULL;
                ck(cuCtxCreate(&cuContext, CU_CTX_SCHED_BLOCKING_SYNC, cuDevice));
                m_ctxV.push_back({cuContext,szDeviceName});
            }
        }
    };
    CUcontext getContext();
};

#endif // NVIDIADECODER_H
