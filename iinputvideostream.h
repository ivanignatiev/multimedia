#ifndef IINPUTVIDEOSTREAM_H
#define IINPUTVIDEOSTREAM_H

#include "videoframe.h"

class IInputVideoStream
{
public:
    ~IInputVideoStream() {}

    virtual VideoFrameData *getFrameData(unsigned long id) = 0;
    virtual VideoHeader const &getHeader(void) const = 0;
};

#endif // IINPUTVIDEOSTREAM_H
