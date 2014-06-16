#include "videoframe.h"

VideoFrame::VideoFrame(IplImage const *cv_frame)
{
    if (cv_frame)
        this->setCvFrame(cv_frame);
}

VideoFrame::VideoFrame(VideoFrameData const *file_frame, VideoHeader const *header)
{
    if (file_frame)
        this->setFileFrame(file_frame, header);
}

VideoFrame::~VideoFrame()
{
    if (this->qt_frame)
        delete this->qt_frame;
}

void VideoFrame::setFileFrame(VideoFrameData const *file_frame, VideoHeader const *header)
{
    this->qt_frame = new QImage(header->width, header->height, QImage::Format_RGB32);

    int RGB_step = header->width * header->height;
    int i = 0;
    unsigned char r, g, b;
    for (int y = 0; y < header->height; ++y) {
        for (int x = 0; x < header->width; ++x) {
            r = file_frame->data[i];
            g = file_frame->data[RGB_step + i];
            b = file_frame->data[RGB_step * 2 + i];

            this->qt_frame->setPixel(x, y, qRgb(r, g, b));
            ++i;
        }
    }

}

void VideoFrame::setCvFrame(IplImage const *cv_frame)
{
    this->qt_frame = new QImage(cv_frame->width, cv_frame->height, QImage::Format_RGB32);

    unsigned char  red, green, blue;
    unsigned int cvIndex = 0, cvLineStart = 0;
    unsigned int y, x;
    for (y = 0; y < cv_frame->height; ++y) {
        cvIndex = cvLineStart;
        for (x = 0; x < cv_frame->width; ++x) {
            red = cv_frame->imageData[cvIndex + 2];
            green = cv_frame->imageData[cvIndex + 1];
            blue = cv_frame->imageData[cvIndex + 0];

            this->qt_frame->setPixel(x, y, qRgb(red, green, blue));
            cvIndex += 3;
        }
        cvLineStart += cv_frame->widthStep;
    }
}

QImage const &VideoFrame::asQImage(void) const
{
    return *(this->qt_frame);
}

unsigned short VideoFrame::getWidth(void) const
{
    return this->qt_frame->width();
}

unsigned short VideoFrame::getHeight(void) const
{
    return this->qt_frame->height();
}
