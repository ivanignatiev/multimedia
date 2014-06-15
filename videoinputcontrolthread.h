#ifndef VIDEOINPUTCONTROLTHREAD_H
#define VIDEOINPUTCONTROLTHREAD_H

#include <QThread>
#include <QGraphicsView>
#include <QDebug>
#include <QSharedPointer>
#include <sys/time.h>
#include "config.h"
#include "videodevice.h"
#include "qglcanvas.h"

class VideoInputControlThread : public QThread
{
    Q_OBJECT

    void run();

public:
    explicit VideoInputControlThread(VideoDevice *_device, VideoConfig const &_config, QObject *parent = 0);
    ~VideoInputControlThread();

    void stop();

    void startCapturing(void);
    void stopCapturing(void);
    bool isCapturing(void) const;

    void setDevice(VideoDevice *device);
private:

    bool running;
    bool capturing;
    unsigned int fps;
    unsigned long fps_time;

    VideoDevice         *device;
    VideoConfig         videoConfig;

signals:
    void frameChanged(VideoFramePointer frame);

public slots:

};

#endif // VIDEOINPUTCONTROLTHREAD_H
