#include "videodecoder.h"

VideoDecoder::VideoDecoder()
{
}

VideoFramePointer VideoDecoder::processFrameData(VideoFrameData *frameData, VideoHeader const *header)
{
    VideoFramePointer frame = VideoFramePointer(new VideoFrame(frameData, header));
    return frame;
}
