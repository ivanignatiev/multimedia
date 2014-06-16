#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H


#include <QThread>
#include <sys/time.h>
#include "config.h"
#include "qglcanvas.h"
#include "videoframe.h"
#include "videodecoder.h"
#include "iinputvideostream.h"

class VideoPlayer : public QThread
{
    Q_OBJECT

    void run();
public:
    VideoPlayer();
    ~VideoPlayer();

    void stop();

    void startPlaying(IInputVideoStream *input);
    void stopPlaying();

    void setFrameId(unsigned long idFrame);

    unsigned long getFrameId() const;

private:
    bool running;
    bool playing;

    IInputVideoStream   *input;
    int step;
    unsigned long idFrame;

signals:
    void frameChanged(VideoFramePointer frame);
};

#endif // VIDEOPLAYER_H
