#include "videodevice.h"

VideoDevice::VideoDevice(int _device_id)
    : device_id(_device_id),
      device(NULL)
{
}

VideoDevice::~VideoDevice()
{
    this->closeDevice();
}

VideoFrame *VideoDevice::getFrame()
{
    IplImage *frame = cvQueryFrame(this->device);
    if (frame)
        return new VideoFrame(frame);
    return NULL;
}

bool VideoDevice::openDevice()
{
    if (this->device)
        return true;
    this->device = cvCreateCameraCapture(this->device_id);
    return (this->device != NULL);
}

void VideoDevice::closeDevice()
{
    if (this->device) {
        cvReleaseCapture(&this->device);
        this->device = NULL;
    }
}

bool VideoDevice::isOpened()
{
 return (this->device == NULL);
}
