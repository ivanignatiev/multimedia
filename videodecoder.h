#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "videoframe.h"

class VideoDecoder
{
public:
    static VideoFramePointer processFrameData(VideoFrameData *frame, VideoHeader const *header);
private:
    VideoDecoder();
};

#endif // VIDEODECODER_H
