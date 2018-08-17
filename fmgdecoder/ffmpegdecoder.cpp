#include "ffmpegdecoder.h"
#include <iostream>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/frame.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
}

bool FFmpegDecoder::decode(const char* source, std::string &erroStr, std::function<void(AVPixelFormat,unsigned char*,int,int)> frameHandler, std::mutex *mtx)
{
    m_run = true;
    AVFormatContext *pAVFomatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame,*pAVFrameRGB;
    SwsContext *pSwsContext;
    AVPacket *pAVPacket;
    av_register_all();
    avformat_network_init();
    pAVFomatContext = avformat_alloc_context();
    pAVFrame = av_frame_alloc();
    pAVFrameRGB = av_frame_alloc();

    AVDictionary *opt = nullptr;
//    av_dict_set(&opt,"buffer_size","1024000",0);
//    av_dict_set(&opt,"max_delay","0",0);
//    av_dict_set(&opt,"rtsp_transport","tcp",0);
    av_dict_set(&opt,"stimeout","5000000",0);
    int result = avformat_open_input(&pAVFomatContext,source, nullptr, &opt);
    if(result < 0){
        erroStr = av_err2str(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_free_context(pAVFomatContext);
        return false;
    }

    result = avformat_find_stream_info(pAVFomatContext, nullptr);
    if(result < 0){
        erroStr = av_err2str(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avformat_close_input(&pAVFomatContext);
        avformat_free_context(pAVFomatContext);
        return false;
    }

    int videoStreamIndex = -1;
    for(uint i = 0; i < pAVFomatContext->nb_streams; i++){
        if(pAVFomatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
            break;
        }
    }

    if(videoStreamIndex == -1){
        erroStr = av_err2str(result);
        return false;
    }

    pAVCodecContext = pAVFomatContext->streams[videoStreamIndex]->codec;
    int videoWidth = pAVCodecContext->width;
    int videoHeight = pAVCodecContext->height;

    AVCodec *pAVCodec = nullptr;
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
//    pAVCodec = avcodec_find_decoder_by_name("h264_cuvid");//硬解码264
    if(!pAVCodec){
        erroStr = av_err2str(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avcodec_close(pAVCodecContext);
        avformat_close_input(&pAVFomatContext);
        avformat_free_context(pAVFomatContext);
        return false;
    }

    uint8_t *out_buffer{nullptr};
    pSwsContext = sws_getContext(videoWidth,videoHeight,pAVCodecContext->pix_fmt,videoWidth,videoHeight,AV_PIX_FMT_YUV420P,SWS_FAST_BILINEAR,nullptr,nullptr,nullptr);
    int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P,videoWidth,videoHeight);
    out_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture*)pAVFrameRGB,out_buffer,AV_PIX_FMT_YUV420P,videoWidth,videoHeight);

    int y_size = pAVCodecContext->width * pAVCodecContext->height;
    pAVPacket = (AVPacket*) malloc(sizeof(AVPacket));
    av_new_packet(pAVPacket,y_size);
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
        erroStr = av_err2str(result);
        av_frame_free(&pAVFrame);
        av_frame_free(&pAVFrameRGB);
        avcodec_close(pAVCodecContext);
        avformat_free_context(pAVFomatContext);
        return false;
    }

    int resCode = 0;
    int got_picture = 0;
    while (m_run) {
        if((resCode = av_read_frame(pAVFomatContext,pAVPacket)) <0){
//            if(resCode != AVERROR_EOF){
//                emit sigError(QString("av_read_frame error: %1").arg(resCode));
//            }
            break;
        }

        if(pAVPacket->stream_index == videoStreamIndex){
            avcodec_decode_video2(pAVCodecContext,pAVFrame,&got_picture,pAVPacket);
            if(got_picture){
                if(mtx)mtx->lock();
                sws_scale(pSwsContext,(const uint8_t *const*)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVFrameRGB->data,pAVFrameRGB->linesize);
                if(mtx)mtx->unlock();
                frameHandler(AV_PIX_FMT_YUV420P,out_buffer,videoWidth,videoHeight);
            }
        }
        av_free_packet(pAVPacket);
    }

    av_free(out_buffer);
    av_frame_free(&pAVFrame);
    av_frame_free(&pAVFrameRGB);
    avcodec_close(pAVCodecContext);
    avformat_close_input(&pAVFomatContext);
    avformat_free_context(pAVFomatContext);
    return true;
}

void FFmpegDecoder::stop()
{
    m_run = false;
}
