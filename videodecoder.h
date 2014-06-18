#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <vector>
#include "config.h"
#include "dct.h"
#include "yuv.h"
#include "videoframe.h"

class VideoDecoder
{
public:
    static VideoFramePointer processFrameData(VideoFrameData *frame, VideoHeader const *header);
private:
    static unsigned char *previosFrameData;
    static float    dct_cos[DCT_block_size * DCT_block_size];
    static float    dct_quantization[DCT_block_size * DCT_block_size];
    static int      zigzag_way[DCT_block_size * DCT_block_size];
    static float    result[DCT_block_size * DCT_block_size];
    static float    block[DCT_block_size * DCT_block_size];

    static void convertYUVtoRGB(VideoFrameData *data);
    static void merge(VideoFrameData *data, unsigned char *previousData);
    static void iDCT(VideoFrameData *data);
    static void iDCTBlock(unsigned int offset, unsigned int width, unsigned int x, unsigned int y, VideoFrameData *data);
    static void decompresseZeroRLE(VideoFrameData *data);
    VideoDecoder();
};

#endif // VIDEODECODER_H
