#ifndef FILEOUTPUTVIDEOSTREAM_H
#define FILEOUTPUTVIDEOSTREAM_H

#include <exception>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "ioutputvideostream.h"

class FileOutputVideoStream : public IOutputVideoStream
{
public:
    FileOutputVideoStream(QString const &filename, VideoHeader const &_header);
    virtual ~FileOutputVideoStream();

    virtual void pushFrameData(VideoFrameData &videoFrameData);

private:

    void writeFramesIndex(void);

    QString filename;
    QString filepath;

    VideoHeader header;
    std::vector<VideoFrameData> frames;

    FILE *fd;
};

#endif // FILEOUTPUTVIDEOSTREAM_H
