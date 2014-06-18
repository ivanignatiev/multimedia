#ifndef YUV_H
#define YUV_H

#define YUV_KR 0.299
#define YUV_KB 0.114
#define CLIP(x) (((x) < 0) ?  0 : (((x) > 255) ? 255 : (x)))

#endif // YUV_H
