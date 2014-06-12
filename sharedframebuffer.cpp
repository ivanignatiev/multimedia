#include "sharedframebuffer.h"

SharedFrameBuffer::SharedFrameBuffer()
    :
      fps(_DEFAULT_FPS),
      fps_time(_DEFAULT_FPS_TIME),
      frame_index(0)
{
}


SharedFrameBuffer::~SharedFrameBuffer()
{

}

void SharedFrameBuffer::setFramePerSecond(unsigned int fps)
{
    this->fps = fps;
    this->fps_time = 1000000 / fps;
}

unsigned int SharedFrameBuffer::getFPS() const
{
    return this->fps;
}

unsigned long SharedFrameBuffer::getFPSTime() const
{
    return this->fps_time;
}


void SharedFrameBuffer::pushTop(VideoFrame *frame)
{
    delete frame;
}

VideoFrame *SharedFrameBuffer::popBottom(void)
{
    return (NULL);
}
