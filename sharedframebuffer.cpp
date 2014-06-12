#include "sharedframebuffer.h"

SharedFrameBuffer::SharedFrameBuffer()
    :
      fps(_DEFAULT_FPS),
      fps_time(_DEFAULT_FPS_TIME)
{
}

void SharedFrameBuffer::setFramePerSecond(unsigned int fps)
{
    this->fps = fps;
    this->fps_time = 1000000 / fps;
}

unsigned int SharedFrameBuffer::getFPS()
{
    return this->fps;
}

unsigned long SharedFrameBuffer::getFPSTime()
{
    return this->fps_time;
}
