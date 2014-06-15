#include "videodevice.h"

VideoDevice::VideoDevice(int _device_id)
    : device_id(_device_id),
      device(NULL),
      device_mutex(new QMutex())
{
    this->device_mutex->unlock();
}

VideoDevice::~VideoDevice()
{
    this->close();
}

VideoFrame *VideoDevice::getFrame()
{
    this->device_mutex->lock();
    IplImage *frame = cvQueryFrame(this->device);
    if (frame) {
        VideoFrame *vd_frame = new VideoFrame(frame);
        this->device_mutex->unlock();
        return vd_frame;
    }
    this->device_mutex->unlock();
    return NULL;
}

bool VideoDevice::open()
{
    if (this->device)
        return true;
    this->device_mutex->lock();
    this->device = cvCreateCameraCapture(this->device_id);
    this->device_mutex->unlock();
    return (this->device != NULL);
}

void VideoDevice::close()
{
    if (this->device) {
        this->device_mutex->lock();
        cvReleaseCapture(&this->device);
        this->device_mutex->unlock();
        this->device = NULL;
    }
}

bool VideoDevice::isOpened() const
{
    return (this->device == NULL);
}
