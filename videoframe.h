#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>

class VideoFrame
{
public:
    VideoFrame(IplImage *_cv_frame);
    QImage const *asQImage(void);
    ~VideoFrame(void);
private:
    IplImage   *cv_frame;
    QImage      *qt_frame;
};

#endif // VIDEOFRAME_H
