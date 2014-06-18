#include "videoinputcontrolthread.h"

VideoInputControlThread::VideoInputControlThread(VideoDevice *_device, VideoConfig const &_config, QObject *parent) :
    QThread(parent),
    running(true),
    device(_device),
    videoConfig(_config)
{

}

VideoInputControlThread::~VideoInputControlThread()
{
    this->stop();
}


void VideoInputControlThread::run()
{
    struct timeval	start_loop;
    struct timeval	stop_loop;
    unsigned long long	elapsedTime;
    int framesCount = 0;

    while (this->running || framesCount != 0) {
        gettimeofday(&start_loop, NULL);

        if (this->isCapturing()) {
            VideoFramePointer frame = VideoFramePointer(this->device->getFrame());

            emit frameChanged(frame);
        }

        gettimeofday(&stop_loop, NULL);

        elapsedTime = (stop_loop.tv_sec - start_loop.tv_sec) * _USECOND;
        elapsedTime = elapsedTime + (stop_loop.tv_usec - start_loop.tv_usec);

        if (elapsedTime < this->videoConfig.delta_time)
            this->usleep(this->videoConfig.delta_time - elapsedTime);

        framesCount = (framesCount + 1) % this->videoConfig.fps;
    }

}

void VideoInputControlThread::stop() {
    this->running = false;
    this->wait();
}

void VideoInputControlThread::setDevice(VideoDevice *device)
{
    this->device = device;
}

void VideoInputControlThread::startCapturing(void)
{
    this->capturing = this->device->open();
}

void VideoInputControlThread::stopCapturing(void)
{
    this->capturing = false;
    this->device->close();
}

bool VideoInputControlThread::isCapturing(void) const
{
    return this->capturing;
}

