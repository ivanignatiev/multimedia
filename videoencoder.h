#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <omp.h>
#include <vector>
#include "config.h"
#include "dct.h"
#include "yuv.h"
#include "videoframe.h"
#include "huffman.h"

class VideoEncoder
{
public:
    VideoEncoder();
    ~VideoEncoder();

    static VideoFrameData *proccessFrame(VideoFramePointer frame);
private:
    static unsigned char *previosFrameData;
    static float    dct_cos[DCT_block_size * DCT_block_size];
    static float    dct_quantization[DCT_block_size * DCT_block_size];
    static int      zigzag_way[DCT_block_size * DCT_block_size];
    static float    result[8][3][DCT_block_size * DCT_block_size];
    static float    block[8][3][DCT_block_size * DCT_block_size];

    static void diff(VideoFrameData *data, unsigned char *previosFrameData);
    static void convertRGBToYUV(VideoFramePointer frame, VideoFrameData *data);
    static void applyDCT(VideoFrameData *data);
    static void applyDCTToBlock(bool uv, unsigned int x, unsigned int y, VideoFrameData *data);
    static void applyZeroRLE(VideoFrameData *data);
    static void applyStaticHuffman(VideoFrameData *data);
};

#endif // VIDEOENCODER_H
