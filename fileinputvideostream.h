#ifndef FILEINPUTVIDEOSTREAM_H
#define FILEINPUTVIDEOSTREAM_H

#include <QString>
#include <stdio.h>
#include "config.h"
#include "videoframe.h"
#include "iinputvideostream.h"

class FileInputVideoStream : public IInputVideoStream
{
public:
    FileInputVideoStream(QString const &filename);
    ~FileInputVideoStream();

     VideoFrameData *getFrameData(unsigned long id);
     VideoHeader const &getHeader(void) const;
private:

    QString filename;
    QString filepath;
    VideoHeader header;

    FILE    *fd;
};

#endif // FILEINPUTVIDEOSTREAM_H
