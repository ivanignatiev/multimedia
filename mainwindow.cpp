#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    videoConfig(VideoConfig(_DEFAULT_FPS, _USECOND / _DEFAULT_FPS)),
    outputFileStream(NULL),
    inputFileStream(NULL)
{
    ui->setupUi(this);

    this->videoRecorderScreen = new QGLCanvas(this);
    this->ui->videoLayout->addWidget(this->videoRecorderScreen);

    this->videoPlayerScreen = new QGLCanvas(this);
    this->ui->videoPlayerLayout->addWidget(this->videoPlayerScreen);

    this->videoDevice = new VideoDevice(_DEFAULT_DEVICE_ID);
    this->vicThread = new VideoInputControlThread(this->videoDevice, this->videoConfig);
    this->videoRecorder = new VideoRecorder(this->videoConfig);
    this->videoPlayer = new VideoPlayer();

    this->vicThread->start();
    this->videoRecorder->start();
    this->videoPlayer->start();

    qRegisterMetaType<VideoFramePointer>("VideoFramePointer");

    connect(this->vicThread, SIGNAL(frameChanged(VideoFramePointer)), this->videoRecorderScreen, SLOT(changeFrame(VideoFramePointer)));
    connect(this->vicThread, SIGNAL(frameChanged(VideoFramePointer)), this->videoRecorder, SLOT(changeFrame(VideoFramePointer)));
    connect(this->videoPlayer, SIGNAL(frameChanged(VideoFramePointer)), this->videoPlayerScreen, SLOT(changeFrame(VideoFramePointer)));

    connect(this->videoRecorder, SIGNAL(framesProccessed(ulong,ulong)), this, SLOT(framesProccessed(ulong,ulong)));
    connect(this->videoPlayer, SIGNAL(framePlayed(ulong,ulong)), this, SLOT(framePlayed(ulong,ulong)));

}

MainWindow::~MainWindow()
{
    delete ui;

    delete this->vicThread;
    delete this->videoRecorder;
    delete this->videoDevice;
    delete this->videoPlayer;

    if (this->inputFileStream)
        delete this->inputFileStream;

    if (this->outputFileStream)
        delete this->outputFileStream;
}

void MainWindow::on_btn_PlayPause_clicked()
{
    if (this->ui->btn_PlayPause->text() == "Play") {
        this->ui->btn_PlayPause->setText("Pause");
        this->vicThread->startCapturing();
    } else {
        this->ui->btn_PlayPause->setText("Play");
        this->vicThread->stopCapturing();
    }
}

void MainWindow::on_btn_Record_clicked()
{
    if (this->ui->btn_Record->isChecked()) {
        if (this->outputFileStream == NULL) {
            VideoHeader vh;
            vh.content_length = 0;
            vh.fps = _DEFAULT_FPS;
            vh.width = _DEFAULT_SCREEN_WIDTH;
            vh.height = _DEFAULT_SCREEN_HEIGHT;
            vh.version = _FORMAT_VERSION;

            this->outputFileStream = new FileOutputVideoStream("1", vh);
        }

        this->videoRecorder->startRecording(this->outputFileStream);
        this->vicThread->startCapturing();
    } else {
        this->videoRecorder->stopRecording();
    }
}

void MainWindow::on_btn_Stop_clicked()
{
    if (this->outputFileStream) {
        this->vicThread->stopCapturing();
        this->videoRecorder->stopRecording();
        this->videoRecorder->waitToFinish();
        delete this->outputFileStream;
        this->outputFileStream = NULL;
        this->ui->btn_Record->setChecked(false);
        this->ui->btn_PlayPause->setText("Play");
    }
}

void MainWindow::on_btn_PlayPause_4_clicked()
{
    if (this->ui->btn_PlayPause_4->text() == "Play") {
        if (this->inputFileStream == NULL) {
            this->inputFileStream = new FileInputVideoStream("1");
            int seconds = this->inputFileStream->getHeader().frames_count / this->inputFileStream->getHeader().fps;
            this->ui->scroll_VidePlayer->setMaximum(seconds);
        }
        this->ui->btn_PlayPause_4->setText("Pause");
        this->videoPlayer->startPlaying(this->inputFileStream);
    } else {
        this->ui->btn_PlayPause_4->setText("Play");
         this->videoPlayer->stopPlaying();
    }
}

void MainWindow::framesProccessed(unsigned long count, unsigned long buffer)
{
    QString tm;
    unsigned long h, i, s;

    count /= _DEFAULT_FPS;
    buffer /= _DEFAULT_FPS;

    h = count / 3600;
    i = (count - h * 3600) / 60;
    s = (count - h * 3600 - i * 60) ;

    tm.sprintf("%02d:%02d:%02d", h, i, s);
    this->ui->label_RecordedTime->setText(tm);

    h = buffer / 3600;
    i = (buffer - h * 3600) / 60;
    s = (buffer - h * 3600 - i * 60) ;

    tm.sprintf("%02d:%02d:%02d", h, i, s);
    this->ui->label_InBuffer->setText(tm);
}

void MainWindow::framePlayed(unsigned long frame, unsigned long total)
{
    QString tm;
    unsigned long h, i, s, ht, it, st;

    frame /= _DEFAULT_FPS;
    total /= _DEFAULT_FPS;

    h = frame / 3600;
    i = (frame - h * 3600) / 60;
    s = (frame - h * 3600 - i * 60) ;

    ht = total / 3600;
    it = (total - h * 3600) / 60;
    st = (total - h * 3600 - i * 60) ;

    tm.sprintf("%02d:%02d:%02d / %02d:%02d:%02d", h, i, s, ht, it, st);
    this->ui->label_TimePlayed->setText(tm);

    this->ui->scroll_VidePlayer->setValue(frame);

     if (!this->inputFileStream)
        this->ui->label_TimePlayed->setText("00:00:00 / 00:00:00");
}

void MainWindow::on_btn_StopPlayer_clicked()
{
    if (this->inputFileStream) {
        this->videoPlayer->stopPlaying();
        this->videoPlayer->waitToFrameFinish();
        delete this->inputFileStream;
        this->inputFileStream = NULL;
        this->ui->btn_PlayPause_4->setText("Play");
        this->ui->scroll_VidePlayer->setValue(0);
        this->ui->scroll_VidePlayer->setMaximum(0);
        this->ui->label_TimePlayed->setText("00:00:00 / 00:00:00");
    }
}

void MainWindow::on_scroll_VidePlayer_sliderMoved(int position)
{
    if (this->inputFileStream)
        this->videoPlayer->setFrameId(position * this->inputFileStream->getHeader().fps);
    // TODO : find better slot! It's just moving and no clicking
}

void MainWindow::on_tabWidget_3_tabBarClicked(int index)
{
    // TODO if index == 2 Library
}
