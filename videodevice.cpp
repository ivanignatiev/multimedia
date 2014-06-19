#include "videodevice.h"

VideoDevice::VideoDevice(int _device_id)
    : device_id(_device_id),
      device(NULL),
      device_mutex(new QMutex()),
      width(_DEFAULT_SCREEN_WIDTH),
      height(_DEFAULT_SCREEN_HEIGHT)
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
    IplImage *resizedframe = cvCreateImage( cvSize(this->getWidth() , this->getHeight()),
                                         frame->depth, frame->nChannels );
    cvResize(frame, resizedframe);
    if (resizedframe) {
        VideoFrame *vd_frame = new VideoFrame(resizedframe);

        this->device_mutex->unlock();
        cvReleaseImage(&resizedframe);
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
    return (this->device != NULL);
}

bool VideoDevice::setDeviceId(int device_id)
{
    this->device_id = device_id;

    if (this->isOpened()) {
        this->device_mutex->lock();
        cvReleaseCapture(&this->device);
        this->device = cvCreateCameraCapture(this->device_id);
        this->device_mutex->unlock();
        return (this->device != NULL);
    }

    return true;
}

void VideoDevice::setWidth(unsigned int width)
{
    this->width = width;
}

void VideoDevice::setHeight(unsigned int height)
{
    this->height = height;
}

unsigned int VideoDevice::getWidth(void) const
{
    return this->width;
}

unsigned int VideoDevice::getHeight(void) const
{
    return this->height;
}

int VideoDevice::getDeviceId(void) const
{
    return this->device_id;
}
