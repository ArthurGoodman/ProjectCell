#include "graphicsview.h"
#include <QtWidgets>

#include "ui.h"
#include "graphicsscene.h"

GraphicsView::GraphicsView()
{
    setCursor(Qt::OpenHandCursor);
}

void GraphicsView::run()
{
    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), scene(), SLOT(update()));
    timer->start(0);
}

void GraphicsView::keyPressEvent(QKeyEvent *e)
{
    QGraphicsView::keyPressEvent(e);
}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    setCursor(Qt::ClosedHandCursor);

    if(e->buttons() & Qt::LeftButton) lastPos = e->pos();
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        ((GraphicsScene *)Ui::view->scene())->shift(e->pos() - lastPos);
        lastPos = e->pos();
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}

void GraphicsView::wheelEvent(QWheelEvent *e)
{
    ((GraphicsScene *)Ui::view->scene())->zoom(e->delta() > 0 ? 1.2 : 1.0/1.2);
}
