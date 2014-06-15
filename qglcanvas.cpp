#include "qglcanvas.h"

QGLCanvas::QGLCanvas(QWidget* parent)
    : QGLWidget(parent)
{
}

QGLCanvas::~QGLCanvas()
{

}

void QGLCanvas::changeFrame(VideoFramePointer frame)
{
    img = frame->asQImage();
    this->update();

}

void QGLCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    p.drawImage(this->rect(), img);
}
