#include "videoencoder.h"

unsigned char *VideoEncoder::previosFrameData = NULL;
float VideoEncoder::dct_cos[DCT_block_size * DCT_block_size] = { DCT_cos };
float VideoEncoder::dct_quantization[DCT_block_size * DCT_block_size] = { DCT_quantization };
int VideoEncoder::zigzag_way[DCT_block_size * DCT_block_size] = { ZIGZAG_WAY };
float VideoEncoder::result[8][3][DCT_block_size * DCT_block_size];
float VideoEncoder::block[8][3][DCT_block_size * DCT_block_size];

VideoEncoder::VideoEncoder()
{
}

VideoEncoder::~VideoEncoder()
{

}

void VideoEncoder::convertRGBToYUV(VideoFramePointer frame, VideoFrameData *data)
{
    unsigned int Y_step = data->width * data->height;
    unsigned int UV_step = Y_step / 4;

    data->header.content_length = Y_step + UV_step * 2;
    data->data = new unsigned char[data->header.content_length];

    unsigned int i = 0, jy = 0, j = 0, yK = (data->width / 2);
    float L;
    unsigned char Y, U, V;
    unsigned char r, g, b;

    for (int y = 0; y < data->height; ++y) {
        jy = ((y - y % 2) / 2) * yK;
        for (int x = 0; x < data->width; ++x) {
            QRgb rgb = frame->asQImage().pixel(x, y);
            r = qRed(rgb);
            g = qGreen(rgb);
            b = qBlue(rgb);

            L = YUV_KR * r + YUV_KB * b + (1.0 - YUV_KR - YUV_KB) * g;

            Y = (219.0 * L / 255.0) + 16;

            data->data[i] = Y;

            if (x % 2 == 0 && y % 2 == 0) {
                U = (224.0 * 0.5 * (1.0 * b - L) / ((1.0 - YUV_KB) * 255.0)) + 128;
                V = (224.0 * 0.5 * (1.0 * r - L) / ((1.0 - YUV_KR) * 255.0)) + 128;

                j = jy + ((x - x % 2) / 2);

                data->data[Y_step + j] = U;
                data->data[Y_step + UV_step + j] = V;
            }
            ++i;
        }
    }
}

void VideoEncoder::applyDCTToBlock(bool uv, unsigned int x, unsigned int y, VideoFrameData *data)
{
    unsigned int i, j, k;
    float v[3];

    int tid = omp_get_thread_num();

    unsigned int ij, ik, oik, jk, oij;
    unsigned int Y_step = data->width * data->height;
    unsigned int UV_step = Y_step / 4;
    unsigned int UV_width = data->width / 2;

    for (i = 0; i < DCT_block_size; ++i) {

        for (j = 0; j < DCT_block_size; ++j) {

            ij = i * DCT_block_size + j;

            VideoEncoder::result[tid][0][ij] = 0.0;
            VideoEncoder::result[tid][1][ij] = 0.0;
            VideoEncoder::result[tid][2][ij] = 0.0;

            for (k = 0; k < DCT_block_size; ++k) {

                ik = i * DCT_block_size + k;
                oik = (y + k) * data->width + x + j;

                v[0] = (float)data->data[oik];
                VideoEncoder::result[tid][0][ij] += VideoEncoder::dct_cos[ik] * (v[0] - 128.0);

                if (uv) {
                    oik = (y + k) * UV_width + x + j;

                    v[1] = (float)data->data[Y_step + oik];
                    VideoEncoder::result[tid][1][ij] += VideoEncoder::dct_cos[ik] * (v[1] - 128.0);

                    v[2] = (float)data->data[Y_step + UV_step + oik];
                    VideoEncoder::result[tid][2][ij] += VideoEncoder::dct_cos[ik] * (v[2] - 128.0);
                }

            }

        }
    }

    for (i = 0; i < DCT_block_size; ++i) {

        for (j = 0; j < DCT_block_size; ++j) {

            ij = i * DCT_block_size + j;

            VideoEncoder::block[tid][0][ij] = 0.0;
            VideoEncoder::block[tid][1][ij] = 0.0;
            VideoEncoder::block[tid][2][ij] = 0.0;

            for (k = 0; k < DCT_block_size; ++k) {
               ik = i * DCT_block_size + k;
               jk = j * DCT_block_size + k;

               VideoEncoder::block[tid][0][ij] += VideoEncoder::result[tid][0][ik] * VideoEncoder::dct_cos[jk];

               if (uv) {
                VideoEncoder::block[tid][1][ij] += VideoEncoder::result[tid][1][ik] * VideoEncoder::dct_cos[jk];
                VideoEncoder::block[tid][2][ij] += VideoEncoder::result[tid][2][ik] * VideoEncoder::dct_cos[jk];
               }

            }

        }
    }

    for (i = 0; i < DCT_block_size * DCT_block_size; ++i) {
        VideoEncoder::result[tid][0][i] = VideoEncoder::block[tid][0][i] / VideoEncoder::dct_quantization[i];
        VideoEncoder::result[tid][0][i] = (VideoEncoder::result[tid][0][i] >= -0.5 && VideoEncoder::result[tid][0][i] <= 0.5) ? 0.0 : VideoEncoder::result[tid][0][i] + 128.0;

        if (uv) {
            VideoEncoder::result[tid][1][i] = VideoEncoder::block[tid][1][i] / VideoEncoder::dct_quantization[i];
            VideoEncoder::result[tid][1][i] = (VideoEncoder::result[tid][1][i] >= -0.5 && VideoEncoder::result[tid][1][i] <= 0.5) ? 0.0 : VideoEncoder::result[tid][1][i] + 128.0;

            VideoEncoder::result[tid][2][i] = VideoEncoder::block[tid][2][i] / VideoEncoder::dct_quantization[i];
            VideoEncoder::result[tid][2][i] = (VideoEncoder::result[tid][2][i] >= -0.5 && VideoEncoder::result[tid][2][i] <= 0.5) ? 0.0 : VideoEncoder::result[tid][2][i] + 128.0;
        }

    }

    for (i = 0; i < DCT_block_size; ++i) {
        for (j = 0; j < DCT_block_size; ++j) {

            oij = (y + i) * data->width + x + j;
            ij = i * DCT_block_size + j;

            data->data[oij] = (unsigned char)VideoEncoder::result[tid][0][VideoEncoder::zigzag_way[ij]];

            if (uv) {
                oij = (y + i) * UV_width + x + j;

                data->data[Y_step + oij]            = (unsigned char)VideoEncoder::result[tid][1][VideoEncoder::zigzag_way[ij]];
                data->data[Y_step + UV_step + oij]  = (unsigned char)VideoEncoder::result[tid][2][VideoEncoder::zigzag_way[ij]];
            }
        }
    }

}


void VideoEncoder::applyDCT(VideoFrameData *data)
{
    unsigned int x, y;

    unsigned int Y_step = data->width * data->height;
    unsigned int UV_step = Y_step / 4;
    unsigned int UV_width = data->width / 2;
    unsigned int UV_height = data->height / 2;

    #pragma omp parallel for num_threads(8) collapse(2)
    for (y = 0; y < data->height; y += DCT_block_size) {
        for (x = 0; x < data->width; x += DCT_block_size) {
            VideoEncoder::applyDCTToBlock(x < UV_width && y < UV_height, x, y, data);
        }
    }
}

void VideoEncoder::applyZeroRLE(VideoFrameData *data)
{
    unsigned char* rle_result = new unsigned char[data->header.content_length];

    int ci = -1;
    int k = 0;
    unsigned int *pci;
    for (int i = 0; i < data->header.content_length; ++i) {
        if (data->data[i] == 0 && ci == -1) {
            rle_result[k] = 0;
            ++k;
            ci = k;
            rle_result[ci] = 0;
            ++k;
        } else if (data->data[i] == 0) {
            rle_result[ci]++;
            if ( rle_result[ci] == 255) {
                ci = -1;
            }
        } else {
            ci = -1;
            rle_result[k] = data->data[i];
            k++;
        }
    }
    data->header.content_length = k;
    delete data->data;
    data->data = new unsigned char[k];
    std::memcpy(data->data, rle_result, k);
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
    data->width = frame->getWidth();
    data->height = frame->getHeight();

    VideoEncoder::convertRGBToYUV(frame, data);
    VideoEncoder::applyDCT(data);

    if (data->header.type == PFrame) {
        VideoEncoder::diff(data, VideoEncoder::previosFrameData);
    } else if (data->header.type == IFrame) {
        if (VideoEncoder::previosFrameData)
            delete VideoEncoder::previosFrameData;
        unsigned char *preDiffData = new unsigned char[data->header.content_length];
        std::memcpy(preDiffData, data->data, data->header.content_length);
        // TODO : add to destructor
        VideoEncoder::previosFrameData = preDiffData;
    }

    //VideoEncoder::applyZeroRLE(data);

    return data;
}
