#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>
#include <QSharedPointer>


struct VideoHeader {
    unsigned int    version;
    unsigned short  width;
    unsigned short  height;
    unsigned short  fps;
    unsigned long   content_length;
    unsigned long   frames_count;
};

struct VideoFrameHeader {
    unsigned int content_length;
    unsigned char type; // full , diff
    unsigned long data_adress;
};

struct VideoFrameData {
    VideoFrameHeader    header;
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

    ~VideoFrame(void);
private:

    QImage      *qt_frame;
};


typedef QSharedPointer<VideoFrame> VideoFramePointer;

#endif // VIDEOFRAME_H
