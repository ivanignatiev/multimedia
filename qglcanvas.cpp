#include "qglcanvas.h"

QGLCanvas::QGLCanvas(QWidget* parent)
    : QGLWidget(parent)
{
}

QGLCanvas::~QGLCanvas()
{

}

void QGLCanvas::setImage(const QImage &image)
{
    img = image;

    qDebug() << "Show frame on the screen";

}

void QGLCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    p.drawImage(this->rect(), img);
}
