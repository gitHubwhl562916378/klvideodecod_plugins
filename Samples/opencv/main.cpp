#include "opencv.hpp"
#include "decoder.h"
#include <memory>

int main()
{
    std::shared_ptr<Decoder> decoder(Klvideodecod::createDecoder<NvidiaDecoder>());//FFmpegDecoder软解 NvidiaDecoder硬解
    std::string errorStr;
    //最后一个参数可以传入一把锁互斥
    bool isOk = decoder->decode("rtsp://admin:123ABCabc@192.168.2.239/h264/ch1/main/av_stream",errorStr,
                                [](AVPixelFormat format,unsigned char *ptr,int w,int h)->void{
                                    cv::Mat frame;
                                    switch (format) {
                                    case AV_PIX_FMT_NV12:
                                        frame = cv::Mat(3 * h / 2, w,CV_8UC1,ptr);
                                        cv::cvtColor(frame,frame,CV_YUV2BGR_NV12);
                                    break;
                                    case AV_PIX_FMT_YUV420P:
                                        frame = cv::Mat(3 * h / 2, w,CV_8UC1,ptr);
                                        cv::cvtColor(frame,frame,CV_YUV2BGR_I420);
                                    break;
                                    default:
                                    break;
                                    }

                                    cv::imshow("video",frame);
                                    cv::waitKey(1);
                                });
    return 0;
}
