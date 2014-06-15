#include "videoencoder.h"

VideoEncoder::VideoEncoder()
{
}


VideoFrameData *VideoEncoder::proccessFrame(VideoFramePointer frame, VideoConfig const &config)
{
    VideoFrameData *data = new VideoFrameData();
    data->header.content_length = frame->getWidth() * frame->getHeight() * 3;
    data->header.data_adress = 0;
    data->header.type = 0;

    data->data = new unsigned char[data->header.content_length];

    int i = 0;
    for (int y = 0; y < frame->getHeight(); ++y) {
        for (int x = 0; x < frame->getWidth(); ++x) {
            QRgb rgb = frame->asQImage().pixel(x, y);
            data->data[i] = qRed(rgb);
            data->data[i + 1] = qGreen(rgb);
            data->data[i + 2] = qBlue(rgb);
            i += 3;
        }
    }

    return data;
}
