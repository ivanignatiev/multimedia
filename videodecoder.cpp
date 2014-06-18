#include "videodecoder.h"

unsigned char *VideoDecoder::previosFrameData = NULL;
float VideoDecoder::dct_cos[DCT_block_size * DCT_block_size] = { DCT_cos };
float VideoDecoder::dct_quantization[DCT_block_size * DCT_block_size] = { DCT_quantization };
int VideoDecoder::zigzag_way[DCT_block_size * DCT_block_size] = { ZIGZAG_WAY };
float VideoDecoder::result[DCT_block_size * DCT_block_size] = { 0 };
float VideoDecoder::block[DCT_block_size * DCT_block_size] = { 0 };

VideoDecoder::VideoDecoder()
{
}

void VideoDecoder::convertYUVtoRGB(VideoFrameData *frameData)
{
    unsigned int Y_step = frameData->width * frameData->height;
    unsigned int UV_step = Y_step / 4;
    unsigned int RGB_step = Y_step;

    unsigned char *RGB_data = new unsigned char[RGB_step * 3];

    unsigned int i = 0, j = 0, jy = 0;
    int C, D, E;
    unsigned int r, g, b;
    unsigned int yK = (frameData->width / 2);

    for (int y = 0; y < frameData->height; ++y) {
        jy = ((y - y % 2) / 2) * yK;
        for (int x = 0; x < frameData->width; ++x) {
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

            //RGB_data[i] = RGB_data[RGB_step + i] = RGB_data[RGB_step * 2 + i] = (unsigned char)C;


            ++i;
        }
    }

    delete frameData->data;
    frameData->data = RGB_data;
}

void VideoDecoder::iDCTBlock(unsigned int offset, unsigned int width, unsigned int x, unsigned int y, VideoFrameData *data)
{
    unsigned int i, j, k;
    float v;

    for (i = 0; i < DCT_block_size; ++i) {
        for (j = 0; j < DCT_block_size; ++j) {
            VideoDecoder::block[VideoDecoder::zigzag_way[i * DCT_block_size + j]] = (float)data->data[offset + (y + i) * width + j + x];
        }
    }


    for (i = 0; i < DCT_block_size; ++i) {
        for (j = 0; j < DCT_block_size; ++j) {
            VideoDecoder::result[i * DCT_block_size + j] = 0;
            for (k = 0; k < DCT_block_size; ++k) {
                v = VideoDecoder::block[k * DCT_block_size + j];
                if (v > 0.99)
                    VideoDecoder::result[i * DCT_block_size + j] += VideoDecoder::dct_cos[k * DCT_block_size + i] * ((v - 128.0) * VideoDecoder::dct_quantization[k * DCT_block_size + j]);
            }
        }
    }

    for (i = 0; i < DCT_block_size; ++i) {
        for (j = 0; j < DCT_block_size; ++j) {
            VideoDecoder::block[i * DCT_block_size + j] = 0;
            for (k = 0; k < DCT_block_size; ++k) {
               VideoDecoder::block[i * DCT_block_size + j] += VideoDecoder::result[i * DCT_block_size + k] * VideoDecoder::dct_cos[k * DCT_block_size + j];
            }
        }
    }

    for (i = 0; i < DCT_block_size; ++i) {
        for (j = 0; j < DCT_block_size; ++j) {
            data->data[offset + (y + i) * width + j + x] = VideoDecoder::block[i * DCT_block_size + j] + 128;
        }
    }

}

void VideoDecoder::iDCT(VideoFrameData *data)
{
    unsigned int x, y;

    unsigned int Y_step = data->width * data->height;
    unsigned int UV_step = Y_step / 4;
    unsigned int UV_width = data->width / 2;
    unsigned int UV_height = data->height / 2;

    for (y = 0; y < data->height; y += DCT_block_size) {
        for (x = 0; x < data->width; x += DCT_block_size) {
            VideoDecoder::iDCTBlock(0, data->width, x, y, data);

            if (x < UV_width && y < UV_height) {
                VideoDecoder::iDCTBlock(Y_step, UV_width, x, y, data);
                VideoDecoder::iDCTBlock(Y_step + UV_step, UV_width, x, y, data);
            }
        }
    }
}

void VideoDecoder::merge(VideoFrameData *frameData, unsigned char *previousData)
{
    for (int i = 0; i < frameData->header.content_length; ++i) {
        frameData->data[i] = frameData->data[i] ^ previousData[i];
    }
}

void VideoDecoder::decompresseZeroRLE(VideoFrameData *data)
{
    unsigned int Y_step = data->width * data->height;
    unsigned int UV_step = Y_step / 4;
    unsigned int content_length = Y_step * 3;
    unsigned char *result = new unsigned char[content_length];

    int i, j, k = 0;
    for (i = 0; i < data->header.content_length; ++i) {
        if (data->data[i] == 0) {
            result[k] = 0;
            ++k;
            for (j = 0; j < data->data[i + 1]; ++j) {
                result[k] = 0;
                ++k;
            }
            ++i;
        } else {
            result[k] = data->data[i];
            ++k;
        }
    }

    delete data->data;
    std::memcpy(data->data, result, k);
    data->header.content_length = k;
}

VideoFramePointer VideoDecoder::processFrameData(VideoFrameData *frameData, VideoHeader const *header)
{
    frameData->width = header->width;
    frameData->height = header->height;

    //VideoDecoder::decompresseZeroRLE(frameData);

    if (frameData->header.type == PFrame) {
        VideoDecoder::merge(frameData, VideoDecoder::previosFrameData);
    } else if(frameData->header.type == IFrame) {
        if (VideoDecoder::previosFrameData)
            delete VideoDecoder::previosFrameData;
        // TODO : delete destructor
        VideoDecoder::previosFrameData = new unsigned char[frameData->header.content_length];
        std::memcpy(VideoDecoder::previosFrameData, frameData->data, frameData->header.content_length);
    }

    VideoDecoder::iDCT(frameData);
    VideoDecoder::convertYUVtoRGB(frameData);

    VideoFramePointer frame = VideoFramePointer(new VideoFrame(frameData, header));

    return frame;
}
