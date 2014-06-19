#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>
#include <QSharedPointer>
#include "config.h"

enum VideoFrameType {IFrame, PFrame};

struct VideoHeader {
    unsigned int    version;
    unsigned short  width;
    unsigned short  height;
    unsigned short  fps;
    unsigned long   content_length;
    unsigned long   frames_count;
};

struct VideoFrameHeader {
    unsigned long content_length;
    VideoFrameType type;
    unsigned long data_adress;
};

struct VideoFrameData {
    VideoFrameHeader    header;
    unsigned int width;
    unsigned int height;
    unsigned char       *data;
};



class VideoFrame
{
public:
    VideoFrame(IplImage const *_cv_frame = NULL);
    VideoFrame(VideoFrameData const *_file_frame, VideoHeader const *_header);

    void setCvFrame(IplImage const *_cv_frame);
    void setFileFrame(VideoFrameData const *_cv_frame, VideoHeader const *_header);

    QImage const &asQImage(void) const;

    unsigned short getWidth(void) const;
    unsigned short getHeight(void) const;

    void setType(VideoFrameType type);
    VideoFrameType getType(void) const;

    ~VideoFrame(void);
private:

    QImage      *qt_frame;
    VideoFrameType type;
};


typedef QSharedPointer<VideoFrame> VideoFramePointer;
typedef QSharedPointer<VideoFrameData> VideoFrameDataPointer;

#endif // VIDEOFRAME_H
