#ifndef IOUTPUTVIDEOSTREAM_H
#define IOUTPUTVIDEOSTREAM_H

#include "videoframe.h"

class IOutputVideoStream
{
public:
    virtual ~IOutputVideoStream() {}
    virtual void pushFrameData(VideoFrameData *videoFrameData) = 0;
};

#endif // IOUTPUTVIDEOSTREAM_H
