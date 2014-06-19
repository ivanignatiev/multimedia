#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <vector>
#include <QHash>
#include "config.h"
#include "dct.h"
#include "yuv.h"
#include "videoframe.h"
#include "huffman.h"

class VideoDecoder
{
public:
    static VideoFrame *processFrameData(VideoFrameData *frame, VideoHeader const *header);
    static void prepareDecoder();
private:
    static unsigned char *previosFrameData;
    static float    dct_cos[DCT_block_size * DCT_block_size];
    static float    dcty_quantization[DCT_block_size * DCT_block_size];
    static int      zigzag_way[DCT_block_size * DCT_block_size];
    static float    result[DCT_block_size * DCT_block_size];
    static float    block[DCT_block_size * DCT_block_size];
    static float    dctuv_quantization[DCT_block_size * DCT_block_size];

    static unsigned short huffman[256];
    static unsigned short huffman_size[256];
    static QHash<hufkey, unsigned char> huffman_decompressor;

    static void convertYUVtoRGB(VideoFrameData *data);
    static void merge(VideoFrameData *data, unsigned char *previousData);
    static void iDCT(VideoFrameData *data);
    static void iDCTBlock(unsigned int offset, unsigned int width, unsigned int x, unsigned int y, VideoFrameData *data);
    static void decompresseZeroRLE(VideoFrameData *data);
    static void decompresseStaticHuffman(VideoFrameData *data);


    VideoDecoder();
};

#endif // VIDEODECODER_H
