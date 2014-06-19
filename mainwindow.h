#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QSharedPointer>
#include <QInputDialog>
#include "videoinputcontrolthread.h"
#include "videorecorder.h"
#include "videodevice.h"
#include "config.h"
#include "qglcanvas.h"
#include "videoframe.h"
#include "videoplayer.h"
#include "fileoutputvideostream.h"
#include "fileinputvideostream.h"
#include "videolibrary.h"

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

    void on_btn_Record_clicked();

    void on_btn_Stop_clicked();

    void on_btn_PlayPause_4_clicked();

    void on_btn_StopPlayer_clicked();

    void framesProccessed(unsigned long count, unsigned long buffer);

    void framePlayed(unsigned long frame, unsigned long total);

    void on_actionDevice_triggered();

    void on_tabWidget_3_currentChanged(int index);

    void on_list_Videos_doubleClicked(const QModelIndex &index);

    void on_slider_VideoPlayer_valueChanged(int value);


    void on_btn_Backward_clicked();

    void on_btn_Forward_clicked();

private:

    VideoInputControlThread *vicThread;
    VideoRecorder *videoRecorder;
    VideoPlayer *videoPlayer;
    VideoDevice *videoDevice;
    Ui::MainWindow *ui;
    VideoLibrary *videoLibrary;

    QGLCanvas   *videoRecorderScreen;
    QGLCanvas   *videoPlayerScreen;

    VideoConfig videoConfig;
    FileOutputVideoStream *outputFileStream;
    FileInputVideoStream *inputFileStream;

    void playFile(QFileInfo const &file);
    void stopPlaying(void);
    QFileInfo currentFile;
};

#endif // MAINWINDOW_H
