#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frameBuffer(new SharedFrameBuffer())
{
    ui->setupUi(this);

    this->videoScreen = new QGLCanvas(this);
    //this->videoScreen->resize(_DEFAULT_SCREEN_WIDTH, _DEFAULT_SCREEN_HEIGHT);
    this->ui->videoLayout->addWidget(this->videoScreen);

    this->videoDevice = new VideoDevice(_DEFAULT_DEVICE_ID);
    this->vicThread = new VideoInputControlThread(this->videoDevice, this->videoScreen, this->frameBuffer);
    this->videoRecorder = new VideoRecorder(this->frameBuffer);

    this->vicThread->start();
    this->videoRecorder->start();
}

MainWindow::~MainWindow()
{
    delete ui;

    this->vicThread->stopLoop();

    delete this->vicThread;
    delete this->videoRecorder;
    delete this->frameBuffer;
    delete this->videoDevice;
}

void MainWindow::on_btn_PlayPause_clicked()
{
    if (this->ui->btn_PlayPause->text() == "Play") {
        this->ui->btn_PlayPause->setText("Pause");
        this->vicThread->showFramesOnScreen();
    } else {
        this->ui->btn_PlayPause->setText("Play");
        this->vicThread->hideFramesFromScreen();
    }
}

void MainWindow::on_btn_Record_clicked()
{
    if (this->ui->btn_Record->isChecked()) {
        this->vicThread->startPushFramesToBuffer();
    } else {
        this->vicThread->stopPushFramesToBuffer();
    }
}
