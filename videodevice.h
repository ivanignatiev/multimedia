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
    bool setDeviceId(int device_id);
    int getDeviceId(void) const;

    void setWidth(unsigned int width);
    void setHeight(unsigned int height);

    unsigned int getWidth(void) const;
    unsigned int getHeight(void) const;
private:
    int    device_id;

    QMutex              *device_mutex;
    CvCapture           *device;

    unsigned int width;
    unsigned int height;

};

#endif // VIDEODEVICE_H
