#include "videoplayer.h"

VideoPlayer::VideoPlayer() :
    idFrame(0),
    input(NULL),
    running(true)
{

}

VideoPlayer::~VideoPlayer()
{
    this->stop();
}

void VideoPlayer::stop()
{
    this->running = false;
    this->wait();
}

void VideoPlayer::run()
{
    struct timeval	start_loop;
    struct timeval	stop_loop;
    unsigned long long	elapsedTime;

    while (this->running) {
        if (this->input != NULL) {
            gettimeofday(&start_loop, NULL);

            VideoFrameData *frameData = this->input->getFrameData(this->idFrame);

            VideoFramePointer frame = VideoDecoder::processFrameData(frameData, &(this->input->getHeader()));

            emit frameChanged(frame);

            this->idFrame = (this->idFrame + 1 ) % this->input->getHeader().frames_count;

            gettimeofday(&stop_loop, NULL);

            elapsedTime = (stop_loop.tv_sec - start_loop.tv_sec) * 1000000;
            elapsedTime = elapsedTime + (stop_loop.tv_usec - start_loop.tv_usec);

            if (elapsedTime < ( _USECOND / this->input->getHeader().fps))
                this->usleep(( _USECOND / this->input->getHeader().fps) - elapsedTime);

        } else {
            this->sleep(1);
        }
    }
}

void VideoPlayer::startPlaying(IInputVideoStream *input)
{
    running = true;
    if (this->input != input) {
        this->idFrame = 0;
    }
    this->input = input;
}

void VideoPlayer::stopPlaying()
{
    running = false;
}


