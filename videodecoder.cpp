#include "videodecoder.h"

#define CLIP(x) (((x) < 0) ?  0 : (((x) > 255) ? 255 : (x)))

unsigned char *VideoDecoder::previosFrameData = NULL;

VideoDecoder::VideoDecoder()
{
}

void VideoDecoder::convertYUVtoRGB(VideoFrameData *frameData, VideoHeader const *header)
{
    int Y_step = header->width * header->height;
    int UV_step = Y_step / 4;
    int RGB_step = Y_step;

    unsigned char *RGB_data = new unsigned char[RGB_step * 3];

    int i = 0, j = 0, jy = 0;
    int C, D, E;
    int r, g, b;
    for (int y = 0; y < header->height; ++y) {
        jy = ((y - y % 2) / 2) * (header->width / 2);
        for (int x = 0; x < header->width; ++x) {
            C = frameData->data[i] - 16;

            j = jy + ((x - x % 2) / 2);
            D = frameData->data[Y_step + j] - 128;
            E = frameData->data[Y_step + UV_step + j] - 128;

            r = CLIP( 1.164383 * C + 1.596027 * E  );
            g = CLIP( 1.164383 * C - (0.391762 * D) - (0.812968 * E) );
            b = CLIP( 1.164383 * C + 2.017232 * D );

            RGB_data[i] = (unsigned char)r;
            RGB_data[RGB_step + i] = (unsigned char)g;
            RGB_data[RGB_step * 2 + i] = (unsigned char)b;

            //RGB_data[i] = RGB_data[RGB_step + i] = RGB_data[RGB_step * 2 + i] = (unsigned char)E;


            ++i;
        }
    }

    delete frameData->data;
    frameData->data = RGB_data;
}

void VideoDecoder::merge(VideoFrameData *frameData, unsigned char *previousData)
{
    for (int i = 0; i < frameData->header.content_length; ++i) {
        frameData->data[i] = frameData->data[i] ^ previousData[i];
    }
}

VideoFramePointer VideoDecoder::processFrameData(VideoFrameData *frameData, VideoHeader const *header)
{

    if (frameData->header.type == PFrame) {
        VideoDecoder::merge(frameData, VideoDecoder::previosFrameData);
    } else if(frameData->header.type == IFrame) {
        if (VideoDecoder::previosFrameData)
            delete VideoDecoder::previosFrameData;
        // TODO : delete destructor
        VideoDecoder::previosFrameData = new unsigned char[frameData->header.content_length];
        std::memcpy(VideoDecoder::previosFrameData, frameData->data, frameData->header.content_length);
    }

    VideoDecoder::convertYUVtoRGB(frameData, header);

    VideoFramePointer frame = VideoFramePointer(new VideoFrame(frameData, header));

    return frame;
}
