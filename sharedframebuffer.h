#ifndef SHAREDFRAMEBUFFER_H
#define SHAREDFRAMEBUFFER_H

#include "config.h"

class SharedFrameBuffer
{
public:
    SharedFrameBuffer();

    void setFramePerSecond(unsigned int fps);
    unsigned int getFPS();
    unsigned long getFPSTime();
private:
    unsigned int fps;
    unsigned long fps_time;
};

#endif // SHAREDFRAMEBUFFER_H
