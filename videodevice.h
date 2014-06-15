#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <QMutex>
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

    bool open();
    void close();
    bool isOpened() const;

private:
    int    device_id;

    QMutex              *device_mutex;
    CvCapture           *device;
};

#endif // VIDEODEVICE_H
