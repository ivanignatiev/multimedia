#include "videorecorder.h"

VideoRecorder::VideoRecorder(VideoConfig const &_config, QObject *parent) :
    QThread(parent),
    framesBuffer(new std::queue<VideoFramePointer>()),
    framesBufferMutex(new QMutex()),
    running(true),
    videoConfig(_config),
    out(NULL)
{
}

VideoRecorder::~VideoRecorder()
{
    this->stop();
    delete this->framesBuffer;
}

void VideoRecorder::stop()
{
    this->running = false;
    this->wait();
}

void VideoRecorder::run()
{
    while (this->running || this->framesBuffer->size() > 0) {
        if (this->framesBuffer->size() > 0 && this->out) {
            this->framesBufferMutex->lock();
            VideoFramePointer videoFrame = this->framesBuffer->back();
            this->framesBuffer->pop();
            this->framesBufferMutex->unlock();

            VideoFrameData *frame = VideoEncoder::proccessFrame(videoFrame, this->videoConfig);
            this->out->pushFrameData(*frame);

        } else {
            this->usleep(this->videoConfig.delta_time);
        }
    }
}

void VideoRecorder::startRecording(IOutputVideoStream *out)
{
    this->recording = true;
    if (this->out == NULL) {
        this->out = out;
    } else if (this->out != out) {
        this->stopRecording();
        this->out = out;
        this->recording = true;
    }
}

void VideoRecorder::stopRecording(void)
{
    this->recording = false;
    while (this->framesBuffer->size() > 0);
}

bool VideoRecorder::isRecording(void) const
{
    return this->recording;
}

void VideoRecorder::changeFrame(VideoFramePointer frame)
{
    if (this->isRecording()) {
        this->framesBufferMutex->lock();
        this->framesBuffer->push(frame);
        this->framesBufferMutex->unlock();
    }
}
