#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QThread>
#include <QMutex>
#include <queue>
#include "config.h"
#include "videoframe.h"
#include "ioutputvideostream.h"
#include "videoencoder.h"

class VideoRecorder : public QThread
{
    Q_OBJECT

    void run();

public:
    explicit VideoRecorder(VideoConfig const &_config, QObject *parent = 0);
    ~VideoRecorder();

    void startRecording(IOutputVideoStream *out);
    void stopRecording();
    bool isRecording() const;
    void waitToFinish();

    void stop();
private:


    bool running;
    bool recording;
    std::queue<VideoFramePointer> *framesBuffer;
    QMutex  *framesBufferMutex;
    VideoConfig videoConfig;
    IOutputVideoStream *out;
    unsigned long idFrame;

signals:
    void framesProccessed(unsigned long count, unsigned long buffer);
public slots:
    void changeFrame(VideoFramePointer frame);

};

#endif // VIDEORECORDER_H
