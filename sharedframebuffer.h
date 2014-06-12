#ifndef SHAREDFRAMEBUFFER_H
#define SHAREDFRAMEBUFFER_H

#include "config.h"
#include "videoframe.h"

class SharedFrameBuffer
{
public:
    SharedFrameBuffer();
    ~SharedFrameBuffer();

    void pushTop(VideoFrame *frame);
    VideoFrame *popBottom(void);

    void setFramePerSecond(unsigned int fps);
    unsigned int getFPS() const;
    unsigned long getFPSTime() const;
private:
    unsigned int fps;
    unsigned long fps_time;
    unsigned long frame_index;
};

#endif // SHAREDFRAMEBUFFER_H
