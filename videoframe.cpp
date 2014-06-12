#include "videoframe.h"

VideoFrame::VideoFrame(IplImage *_cv_frame)
    : cv_frame(_cv_frame)
{
    this->qt_frame = new QImage(this->cv_frame->width, this->cv_frame->height, QImage::Format_RGB32);

    unsigned char  red, green, blue;
    unsigned int cvIndex = 0, cvLineStart = 0;
    unsigned int y, x;
    for (y = 0; y < this->cv_frame->height; ++y) {
        cvIndex = cvLineStart;
        for (x = 0; x < this->cv_frame->width; ++x) {
            red = this->cv_frame->imageData[cvIndex + 2];
            green = this->cv_frame->imageData[cvIndex + 1];
            blue = this->cv_frame->imageData[cvIndex + 0];

            this->qt_frame->setPixel(x, y, qRgb(red, green, blue));
            cvIndex += 3;
        }
        cvLineStart += this->cv_frame->widthStep;
    }


}

VideoFrame::~VideoFrame()
{
    delete this->qt_frame;
}

 QImage const *VideoFrame::asQImage(void)
{
    return this->qt_frame;
}
