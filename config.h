#ifndef CONFIG_H
#define CONFIG_H

#include <QDebug>

#define _DEFAULT_FPS        3
#define _USECOND            1000000
#define _DEFAULT_DEVICE_ID  0
#define _DEFAULT_SCREEN_WIDTH   640
#define _DEFAULT_SCREEN_HEIGHT  480
#define _FORMAT_VERSION         1
#define _DEFAULT_RECORDS_FOLDER         "./records/"
#define _DEFAULT_VIDEO_EXT              ".vid"


struct VideoConfig {
    unsigned int fps;
    unsigned long delta_time;

    VideoConfig(unsigned int _fps, unsigned long _delta_time) :
        fps(_fps), delta_time(_delta_time) {}
};

#endif // CONFIG_H
