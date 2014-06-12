#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include <QDebug>
#include <QGLWidget>
#include <QPainter>

namespace Ui {
    class QGLCanvas;
}

class QGLCanvas : public QGLWidget
{

public:

    explicit QGLCanvas(QWidget* parent = NULL);
    ~QGLCanvas();

    void setImage(const QImage &image);

protected:

    void paintEvent(QPaintEvent*);

private:

    QImage img;



};

#endif // QGLCANVAS_H
