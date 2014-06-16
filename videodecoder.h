#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "videoframe.h"

class VideoDecoder
{
public:
    static VideoFramePointer processFrameData(VideoFrameData *frame, VideoHeader const *header);
private:
    static unsigned char *previosFrameData;
    static void convertYUVtoRGB(VideoFrameData *frameData, VideoHeader const *header);
    static void merge(VideoFrameData *frameData, unsigned char *previousData);
    VideoDecoder();
};

#endif // VIDEODECODER_H
