#include "videoencoder.h"

#define YUV_KR 0.299
#define YUV_KB 0.114

unsigned char *VideoEncoder::previosFrameData = NULL;

VideoEncoder::VideoEncoder()
{
}

void VideoEncoder::convertRGBToYUV(VideoFramePointer frame, VideoFrameData *data)
{
    int Y_step = frame->getWidth() * frame->getHeight();
    int UV_step = Y_step; //Y_step / 2;

    data->header.content_length = Y_step + UV_step * 2;
    data->data = new unsigned char[data->header.content_length];

    int i = 0;
    float L;
    unsigned char Y, U, V;
    unsigned char r, g, b;
    for (int y = 0; y < frame->getHeight(); ++y) {
        for (int x = 0; x < frame->getWidth(); ++x) {
            QRgb rgb = frame->asQImage().pixel(x, y);
            r = qRed(rgb);
            g = qGreen(rgb);
            b = qBlue(rgb);

            L = YUV_KR * r + YUV_KB * b + (1.0 - YUV_KR - YUV_KB) * g;

            Y = (219.0 * L / 255.0) + 16;
            U = (224.0 * 0.5 * (1.0 * b - L) / ((1.0 - YUV_KB) * 255.0)) + 128;
            V = (224.0 * 0.5 * (1.0 * r - L) / ((1.0 - YUV_KR) * 255.0)) + 128;

            data->data[i] = Y;
            data->data[Y_step + i] = U;
            data->data[Y_step + UV_step + i] = V;
            ++i;
        }
    }
}


void VideoEncoder::diff(VideoFrameData *data, unsigned char *previosFrameData)
{
    for (int i = 0; i < data->header.content_length; ++i) {
        data->data[i] = data->data[i] ^ previosFrameData[i];
    }
}

VideoFrameData *VideoEncoder::proccessFrame(VideoFramePointer frame)
{
    VideoFrameData *data = new VideoFrameData();
    data->header.data_adress = 0;
    data->header.type = frame->getType();

    VideoEncoder::convertRGBToYUV(frame, data);

    if (frame->getType() == PFrame) {
        VideoEncoder::diff(data, VideoEncoder::previosFrameData);
    } else if (frame->getType() == IFrame) {
        if (VideoEncoder::previosFrameData)
            delete VideoEncoder::previosFrameData;
        unsigned char *preDiffData = new unsigned char[data->header.content_length];
        std::memcpy(preDiffData, data->data, data->header.content_length);
        // TODO : add to destructor
        VideoEncoder::previosFrameData = preDiffData;
    }

    return data;
}
