#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <exception>
#include "videoframe.h"


/**
 * Singleton for each of device_id
 * @brief The VideoDevice class
 */
class VideoDevice
{

public:
    VideoDevice(int _device_id);
    ~VideoDevice();

    VideoFrame *getFrame();

    bool openDevice();
    void closeDevice();
    bool isOpened();

private:
    int    device_id;


    CvCapture           *device;
};

#endif // VIDEODEVICE_H
