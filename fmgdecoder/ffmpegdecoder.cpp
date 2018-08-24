#include "ffmpegdecoder.h"
#include <iostream>
#include <QDebug>

FFmpegDecoder::~FFmpegDecoder()
{
    if(m_mtx){
        m_mtx->lock();
        av_free(m_buffer);
        m_buffer = nullptr;
        m_mtx->unlock();
    }else{
        av_free(m_buffer);
        m_buffer = nullptr;
    }
}

unsigned char *FFmpegDecoder::framePtr()
{
    return m_buffer;
}

bool FFmpegDecoder::decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx)
{
    m_run = true;
    m_mtx = mtx;
    static bool isInited = false;
    if(!isInited){
        av_register_all();
        avformat_network_init();
        isInited = true;
    }
    AVFrame *pAVFrame{nullptr},*pAVFrameRGB{nullptr};
    AVFormatContext *pAVFomatContext{nullptr};
    AVCodecContext *pAVCodecContext{nullptr};
    SwsContext *pSwsContext{nullptr};
    AVPacket pAVPacket;
    pAVFomatContext = avformat_alloc_context();
    pAVFrame = av_frame_alloc();
    pAVFrameRGB = av_frame_alloc();

    AVDictionary *opt = nullptr;
//    av_dict_set(&opt,"buffer_size","1024000",0);
//    av_dict_set(&opt,"max_delay","0",0);
    av_dict_set(&opt,"rtsp_transport","tcp",0);
    av_dict_set(&opt,"stimeout","5000000",0);
    int result = avformat_open_input(&pAVFomatContext,source, nullptr, &opt);
    if(result < 0){
        erroStr += "open input failed errorCode: ";
        erroStr += std::to_string(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_close_input(&pAVFomatContext);
        return false;
    }

    result = avformat_find_stream_info(pAVFomatContext, nullptr);
    if(result < 0){
        erroStr += "find video stream failed errorCode: ";
        erroStr += std::to_string(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_close_input(&pAVFomatContext);
        return false;
    }

    int videoStreamIndex = -1;
    for(int i = 0; i < pAVFomatContext->nb_streams; i++){
        if(pAVFomatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
            break;
        }
    }

    if(videoStreamIndex == -1){
        erroStr += "find video stream index failed errorCode: ";
        erroStr += std::to_string(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_close_input(&pAVFomatContext);
        return false;
    }

    pAVCodecContext = pAVFomatContext->streams[videoStreamIndex]->codec;
    int videoWidth = pAVCodecContext->width;
    int videoHeight = pAVCodecContext->height;

    AVCodec *pAVCodec = nullptr;
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
//    pAVCodec = avcodec_find_decoder_by_name("h264_cuvid");//硬解码264
    if(!pAVCodec){
        erroStr += "find avcodec failed errorCode: ";
        erroStr += std::to_string(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_close_input(&pAVFomatContext);
        return false;
    }

    pSwsContext = sws_getContext(videoWidth,videoHeight,pAVCodecContext->pix_fmt,videoWidth,videoHeight,AV_PIX_FMT_YUV420P,SWS_FAST_BILINEAR,nullptr,nullptr,nullptr);
    int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P,videoWidth,videoHeight);
    m_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture*)pAVFrameRGB,m_buffer,AV_PIX_FMT_YUV420P,videoWidth,videoHeight);

//    int y_size = pAVCodecContext->width * pAVCodecContext->height;
//    pAVPacket = av_packet_alloc();
//    av_new_packet(pAVPacket,y_size);
    av_init_packet(&pAVPacket);
    pAVPacket.data = nullptr;
    pAVPacket.size = 0;
    av_dump_format(pAVFomatContext,0,source,0);

    AVStream *stream = pAVFomatContext->streams[videoStreamIndex];
    int vden = stream->avg_frame_rate.den,vnum = stream->avg_frame_rate.num;
    int video_fps;
    if(vden <= 0 || vnum <= 0){
        video_fps = 25;
        std::cout << "use default " << video_fps << std::endl;
    }else{
        video_fps = vnum/vden;
        std::cout << "video fps:" << video_fps << std::endl;
    }

    result = avcodec_open2(pAVCodecContext,pAVCodec,nullptr);
    if(result < 0){
        erroStr += "avcodec open failed errorCode: ";
        erroStr += std::to_string(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        av_free(m_buffer);
        sws_freeContext(pSwsContext);
        if(pAVPacket.data){
            av_packet_unref(&pAVPacket);
        }
        avformat_close_input(&pAVFomatContext);
        return false;
    }

    int resCode = 0;
    int got_picture = 0;
    while (m_run) {
        if((resCode = av_read_frame(pAVFomatContext,&pAVPacket)) <0){
//            if(resCode != AVERROR_EOF){
//                emit sigError(QString("av_read_frame error: %1").arg(resCode));
//            }
            av_packet_unref(&pAVPacket);
            break;
        }

        if(pAVPacket.stream_index == videoStreamIndex){
            avcodec_decode_video2(pAVCodecContext,pAVFrame,&got_picture,&pAVPacket);
            if(got_picture){
                if(mtx){
                    mtx->lock();
                    sws_scale(pSwsContext,(const uint8_t *const*)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVFrameRGB->data,pAVFrameRGB->linesize);
                    mtx->unlock();
                }else{
                    sws_scale(pSwsContext,(const uint8_t *const*)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVFrameRGB->data,pAVFrameRGB->linesize);
                }
                frameHandler(AV_PIX_FMT_YUV420P,m_buffer,videoWidth,videoHeight);
            }
        }
        av_packet_unref(&pAVPacket);
    }

    av_frame_free(&pAVFrame);
    av_frame_free(&pAVFrameRGB);
    sws_freeContext(pSwsContext);
    if(pAVPacket.data){
        av_packet_unref(&pAVPacket);
    }
    avformat_close_input(&pAVFomatContext);

    if(!m_run){
        return true;
    }else{
        if(!::strncmp(source,"rtsp",4)){
            erroStr = "AVERROR_EOF";
            return false;
        }else{
            if(resCode != AVERROR_EOF){
                return false;
            }else{
                return true;
            }
        }
    }
}

void FFmpegDecoder::stop()
{
    m_run = false;
}
