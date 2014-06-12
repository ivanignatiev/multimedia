#include "videorecorder.h"

VideoRecorder::VideoRecorder(SharedFrameBuffer *_frameBuffer, QObject *parent) :
    QThread(parent),
    frameBuffer(_frameBuffer)
{
}


void VideoRecorder::run()
{

}
