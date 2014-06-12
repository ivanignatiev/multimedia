#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include "videoinputcontrolthread.h"
#include "videorecorder.h"
#include "sharedframebuffer.h"
#include "videodevice.h"
#include "config.h"
#include "qglcanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btn_PlayPause_clicked();

private:

    SharedFrameBuffer *frameBuffer;
    VideoInputControlThread *vicThread;
    VideoRecorder *videoRecorder;
    VideoDevice *videoDevice;
    Ui::MainWindow *ui;
    QGLCanvas   *videoScreen;
};

#endif // MAINWINDOW_H
