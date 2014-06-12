#ifndef VIDEOINPUTCONTROLTHREAD_H
#define VIDEOINPUTCONTROLTHREAD_H

#include <QThread>
#include <QGraphicsView>
#include <QDebug>
#include <sys/time.h>
#include "sharedframebuffer.h"
#include "videodevice.h"
#include "qglcanvas.h"

class VideoInputControlThread : public QThread
{
    Q_OBJECT

    void run();

    void showOnScreen(VideoFrame *frame);
    void pushToBuffer(VideoFrame *frame);
public:
    explicit VideoInputControlThread(VideoDevice *_device, QGLCanvas *_screen, SharedFrameBuffer *_frameBuffer, QObject *parent = 0);
    ~VideoInputControlThread();

    void stopLoop();

    bool showFramesOnScreen();
    void hideFramesFromScreen();
    bool isFramesOnScreen() const;

    bool startPushFramesToBuffer();
    void stopPushFramesToBuffer();
    bool isPushingFramesToBuffer() const;

    void setDevice(VideoDevice *device);
private:

    bool inLoop;
    bool displaying;
    bool pushing;

    VideoDevice         *device;
    SharedFrameBuffer   *frameBuffer;
    QGLCanvas           *screen;

signals:
    void frameChanged(QImage *frame);

public slots:

};

#endif // VIDEOINPUTCONTROLTHREAD_H
