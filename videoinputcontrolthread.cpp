#include "videoinputcontrolthread.h"

VideoInputControlThread::VideoInputControlThread(VideoDevice *_device, QGLCanvas *_screen, SharedFrameBuffer *_frameBuffer, QObject *parent) :
    QThread(parent),
    inLoop(true),
    device(_device),
    frameBuffer(_frameBuffer),
    displaying(false),
    pushing(false),
    screen(_screen)
{

}

VideoInputControlThread::~VideoInputControlThread()
{

}


void VideoInputControlThread::showOnScreen(VideoFrame *frame)
{
    if (!frame)
        return;

    this->screen->setImage(*(frame->asQImage()));
    this->screen->update();
}

void VideoInputControlThread::pushToBuffer(VideoFrame *frame)
{
    if (!frame)
        return;
    this->frameBuffer->pushTop(frame);
}

void VideoInputControlThread::run()
{
    struct timeval	start_loop;
    struct timeval	stop_loop;
    unsigned long long	elapsedTime;

    while (this->inLoop) {
        gettimeofday(&start_loop, NULL);
        if (this->isFramesOnScreen() || this->isPushingFramesToBuffer()) {
            VideoFrame *frame = this->device->getFrame();

            if (this->isFramesOnScreen()) {
                this->showOnScreen(frame);
            }

            if (this->isPushingFramesToBuffer()) {
                this->pushToBuffer(frame);
            } else if (frame) {
                delete frame;
            }
        }
        gettimeofday(&stop_loop, NULL);

        elapsedTime = (stop_loop.tv_sec - start_loop.tv_sec) * 1000000;
        elapsedTime = elapsedTime + (stop_loop.tv_usec - start_loop.tv_usec);

        if (elapsedTime < this->frameBuffer->getFPSTime())
            this->usleep(this->frameBuffer->getFPSTime() - elapsedTime);
    }

}

bool VideoInputControlThread::showFramesOnScreen()
{
    return (this->displaying = this->device->openDevice());
}

void VideoInputControlThread::hideFramesFromScreen()
{
    this->displaying = false;
    if (!this->isPushingFramesToBuffer())
        this->device->closeDevice();
}

bool VideoInputControlThread::isFramesOnScreen() const
{
    return this->displaying;
}


bool VideoInputControlThread::startPushFramesToBuffer()
{
    return (this->pushing = this->device->openDevice());
}

void VideoInputControlThread::stopPushFramesToBuffer()
{
    this->pushing = false;
    if (!this->isFramesOnScreen())
        this->device->closeDevice();
}

bool VideoInputControlThread::isPushingFramesToBuffer() const
{
    return this->pushing;
}

void VideoInputControlThread::stopLoop() {
    this->inLoop = false;
    wait();
    // TODO : use default QThread signals
}

