#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QThread>

class AudioRecorder : public QThread
{
    Q_OBJECT
public:
    explicit AudioRecorder(QObject *parent = 0);

signals:

public slots:

};

#endif // AUDIORECORDER_H
