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
    connect(this->videoPlayer, SIGNAL(frameChanged(VideoFramePointer)), this, SLOT(changeFrame(VideoFramePointer)));
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
    this->vicThread->stopCapturing();
    this->videoRecorder->stopRecording();
    delete this->outputFileStream;
    this->outputFileStream = NULL;
    this->ui->btn_Record->setChecked(false);
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

void MainWindow::on_btn_StopPlayer_clicked()
{
    this->videoPlayer->stopPlaying();
    delete this->inputFileStream;
    this->inputFileStream = NULL;
}

void MainWindow::changeFrame(VideoFramePointer frame)
{
    int second = this->videoPlayer->getFrameId() / this->inputFileStream->getHeader().fps;
    this->ui->scroll_VidePlayer->setValue(second);
}

void MainWindow::on_scroll_VidePlayer_sliderMoved(int position)
{
    this->videoPlayer->setFrameId(position * this->inputFileStream->getHeader().fps);
    // TODO : find better slot! It's just moving and no clicking
}
