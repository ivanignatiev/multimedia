#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    videoConfig(VideoConfig(_DEFAULT_FPS, _USECOND / _DEFAULT_FPS)),
    outputFileStream(NULL),
    inputFileStream(NULL),
    videoLibrary(new VideoLibrary(_DEFAULT_RECORDS_FOLDER))
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

    this->ui->list_Videos->setModel(this->videoLibrary->getListOfVideos());

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
            vh.fps = this->videoConfig.fps;
            vh.width = this->videoDevice->getWidth();
            vh.height = this->videoDevice->getHeight();
            vh.version = _FORMAT_VERSION;

            bool ok;
            QString filename = QInputDialog::getText(this, "New video", "name", QLineEdit::Normal, "videofile", &ok);

            try {
                if (ok && !filename.isEmpty())
                    this->outputFileStream = new FileOutputVideoStream(filename, vh);
                else {
                    this->ui->btn_Record->setChecked(false);
                    return;
                }
            } catch (std::exception e) {
                this->ui->btn_Record->setChecked(false);
                return ;
            }

            this->ui->label_RecordingFileName->setText(filename);
        }

        this->videoRecorder->startRecording(this->outputFileStream);
        this->vicThread->startCapturing();
        this->ui->btn_PlayPause->setText("Pause");
    } else {
        this->videoRecorder->stopRecording();
    }
}

void MainWindow::on_btn_Stop_clicked()
{
    this->vicThread->stopCapturing();

    if (this->outputFileStream) {
        this->videoRecorder->stopRecording();
        this->videoRecorder->waitToFinish();
        delete this->outputFileStream;
        this->outputFileStream = NULL;
        this->ui->btn_Record->setChecked(false);
        this->ui->label_RecordingFileName->setText("");
        this->ui->label_InBuffer->setText("00:00:00");
        this->ui->label_RecordedTime->setText("00:00:00");
    }

    this->ui->btn_PlayPause->setText("Play");
}

void MainWindow::stopPlaying()
{
    if (this->inputFileStream) {
        this->videoPlayer->stopPlaying();
        this->videoPlayer->waitToFrameFinish();
        delete this->inputFileStream;
        this->inputFileStream = NULL;
        this->ui->btn_PlayPause_4->setText("Play");
        this->ui->slider_VideoPlayer->setValue(0);
        this->ui->slider_VideoPlayer->setMaximum(0);
        this->ui->label_TimePlayed->setText("00:00:00 / 00:00:00");
    }
}

void MainWindow::on_btn_PlayPause_4_clicked()
{
    this->videoPlayer->setStep(1);
    this->ui->btn_Backward->setChecked(false);
    this->ui->btn_Forward->setChecked(false);

    if (this->ui->btn_PlayPause_4->text() == "Play") {
        this->playFile(this->currentFile);
    } else {
        this->ui->btn_PlayPause_4->setText("Play");
        this->videoPlayer->stopPlaying();
    }
}

void MainWindow::playFile(QFileInfo const &file)
{

    this->currentFile = file;

    try {
        if (this->inputFileStream == NULL) {

            this->inputFileStream = new FileInputVideoStream(this->currentFile.baseName());
            this->ui->label_PlayingFileName->setText(this->currentFile.baseName());
            int seconds = this->inputFileStream->getHeader().frames_count / this->inputFileStream->getHeader().fps;
            this->ui->slider_VideoPlayer->setMaximum(seconds);
        }

        this->ui->btn_PlayPause_4->setText("Pause");
        this->ui->tabWidget_3->setCurrentIndex(0);
        this->videoPlayer->startPlaying(this->inputFileStream);
    } catch (std::exception e) {

    }
}

void MainWindow::framesProccessed(unsigned long count, unsigned long buffer)
{
    if (!this->videoRecorder->isRecording())
        return;

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

    this->ui->slider_VideoPlayer->blockSignals(true);
    this->ui->slider_VideoPlayer->setValue(frame);
    this->ui->slider_VideoPlayer->blockSignals(false);

     if (!this->inputFileStream)
        this->ui->label_TimePlayed->setText("00:00:00 / 00:00:00");
}

void MainWindow::on_btn_StopPlayer_clicked()
{
    this->stopPlaying();
}

void MainWindow::on_actionDevice_triggered()
{
    bool ok;
    int device_id = QInputDialog::getInteger(this, "Change device", "Device ID:", this->videoDevice->getDeviceId(), 0, 100, 1, &ok );

    if (ok)
        this->videoDevice->setDeviceId(device_id);

}

void MainWindow::on_tabWidget_3_currentChanged(int index)
{
    if (index == 1) {
        this->videoLibrary->refresh();
    }
}

void MainWindow::on_list_Videos_doubleClicked(const QModelIndex &index)
{
    this->stopPlaying();
    this->playFile(this->videoLibrary->getFileInfoById(index.row()));
}


void MainWindow::on_slider_VideoPlayer_valueChanged(int value)
{
    if (this->inputFileStream) {
           this->videoPlayer->setFrameId(value * this->inputFileStream->getHeader().fps);
    }
}


void MainWindow::on_btn_Backward_clicked()
{
    if (!this->inputFileStream) {
        this->ui->btn_Backward->setChecked(false);
        return ;
    }

    if (this->ui->btn_Backward->isChecked()) {
        this->ui->btn_Forward->setChecked(false);
        this->videoPlayer->alignToSeconds();
        this->videoPlayer->setStep(- this->inputFileStream->getHeader().fps );
    } else {
        this->videoPlayer->setStep(1);
    }
}

void MainWindow::on_btn_Forward_clicked()
{
    if (!this->inputFileStream) {
        this->ui->btn_Forward->setChecked(false);
        return ;
    }

    if (this->ui->btn_Forward->isChecked()) {
        this->ui->btn_Backward->setChecked(false);
        this->videoPlayer->alignToSeconds();
        this->videoPlayer->setStep( this->inputFileStream->getHeader().fps );
    } else {
        this->videoPlayer->setStep(1);
    }
}
