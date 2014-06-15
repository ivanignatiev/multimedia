#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include "config.h"
#include "videoframe.h"

class VideoEncoder
{
public:
    static VideoFrameData *proccessFrame(VideoFramePointer frame, VideoConfig const &config);
private:
    VideoEncoder();
};

#endif // VIDEOENCODER_H
