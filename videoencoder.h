#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H


#include "config.h"
#include "videoframe.h"

class VideoEncoder
{
public:
    static VideoFrameData *proccessFrame(VideoFramePointer frame);
private:
    static unsigned char *previosFrameData;

    static void diff(VideoFrameData *data, unsigned char *previosFrameData);
    static void convertRGBToYUV(VideoFramePointer frame, VideoFrameData *data);
    VideoEncoder();
};

#endif // VIDEOENCODER_H
