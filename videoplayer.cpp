#include "videoplayer.h"

VideoPlayer::VideoPlayer() :
    idFrame(0),
    input(NULL),
    running(true),
    step(1),
    waitFrame(false)
{
    VideoDecoder::prepareDecoder();
}

VideoPlayer::~VideoPlayer()
{
    this->stop();
}

void VideoPlayer::stop()
{
    this->running = false;
    this->waitToFrameFinish();
    this->wait();
}

void VideoPlayer::waitToFrameFinish(void)
{
    while (this->waitFrame);
}

void VideoPlayer::run()
{
    struct timeval	start_loop;
    struct timeval	stop_loop;
    unsigned long long	elapsedTime;

    while (this->running) {

        if (this->input != NULL && this->playing) {

            this->waitFrame = true;

            gettimeofday(&start_loop, NULL);

            VideoFrameData *frameData = this->input->getFrameData(this->idFrame);

            VideoFramePointer frame = VideoFramePointer(VideoDecoder::processFrameData(frameData, &(this->input->getHeader())));

            emit frameChanged(frame);
            emit framePlayed(this->idFrame, this->input->getHeader().frames_count);

            delete frameData->data;
            delete frameData;

            this->idFrame = this->idFrame + this->step;

            if (this->idFrame >= this->input->getHeader().frames_count)
                this->idFrame = 0;
            else if (this->step < 0 && this->idFrame <= 0)
                this->idFrame = (this->input->getHeader().frames_count / this->input->getHeader().fps) * this->input->getHeader().fps;

            gettimeofday(&stop_loop, NULL);

            elapsedTime = (stop_loop.tv_sec - start_loop.tv_sec) * _USECOND;
            elapsedTime = elapsedTime + (stop_loop.tv_usec - start_loop.tv_usec);

            this->waitFrame = false;

            if (elapsedTime < ( _USECOND / this->input->getHeader().fps))
                this->usleep(( _USECOND / this->input->getHeader().fps) - elapsedTime);

        } else {
            this->sleep(1);
        }

    }
}

void VideoPlayer::startPlaying(IInputVideoStream *input)
{
    this->playing = true;
    if (this->input != input) {
        this->idFrame = 0;
    }
    this->input = input;
}

void VideoPlayer::stopPlaying()
{
    this->playing = false;
}

void VideoPlayer::setFrameId(unsigned long idFrame)
{
    this->waitToFrameFinish();
    this->idFrame = idFrame;
}

unsigned long VideoPlayer::getFrameId(void) const
{
    return this->idFrame;
}

void VideoPlayer::setStep(int step)
{
    this->step = step;
}

int VideoPlayer::getStep(void) const
{
    return this->step;
}

void VideoPlayer::alignToSeconds(void)
{
    this->waitToFrameFinish();
    this->idFrame = ( this->idFrame / this->input->getHeader().fps ) * this->input->getHeader().fps;
}
