#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "videoframe.h"

class VideoDecoder
{
public:
    static VideoFramePointer processFrameData(VideoFrameData *frame, VideoHeader const *header);
private:
    static void convertYUVtoRGB(VideoFrameData *frameData, VideoHeader const *header);
    VideoDecoder();
};

#endif // VIDEODECODER_H
