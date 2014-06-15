#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include <QDebug>
#include <QGLWidget>
#include <QPainter>
#include <QSharedPointer>
#include "videoframe.h"

namespace Ui {
    class QGLCanvas;
}

class QGLCanvas : public QGLWidget
{

    Q_OBJECT

public:

    explicit QGLCanvas(QWidget* parent = NULL);
    ~QGLCanvas();

protected:

    void paintEvent(QPaintEvent*);

private:

    QImage img;

public slots:
    void changeFrame(VideoFramePointer frame);


};

#endif // QGLCANVAS_H
