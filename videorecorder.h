#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QThread>
#include "sharedframebuffer.h"

class VideoRecorder : public QThread
{
    Q_OBJECT

    void run();

public:
    explicit VideoRecorder(SharedFrameBuffer *frameBuffer, QObject *parent = 0);
private:
    SharedFrameBuffer *frameBuffer;
signals:

public slots:

};

#endif // VIDEORECORDER_H
