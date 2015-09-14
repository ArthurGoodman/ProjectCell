#include "graphicsscene.h"
#include <QtWidgets>

#include "kernel.h"
#include "cellengine.h"

GraphicsScene::GraphicsScene()
{
    scale = 1;

    Kernel::engine->randomize();
    Kernel::engine->start();

    showIndicator = true;
    rotateIndicator = true;

    indicatorAngle = 0;

    indicatorTimer = new QTimer;
    connect(indicatorTimer, SIGNAL(timeout()), SLOT(increaseIndicatorAngle()));
    indicatorTimer->start(16);
}

void GraphicsScene::shift(QPoint delta)
{
    offset += delta;
}

void GraphicsScene::zoom(double factor)
{
    scale *= factor;
}

void GraphicsScene::toggleIndicator()
{
    showIndicator = !showIndicator;
}

void GraphicsScene::toggleIndicatorRotation()
{
    rotateIndicator = !rotateIndicator;
    if(!rotateIndicator) indicatorAngle = 0;
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, Qt::lightGray);

    const QImage &image = Kernel::engine->getImage();

    if(showIndicator) painter->save();

    painter->translate(rect.center() - scale*image.rect().center() + offset);
    painter->scale(scale, scale);

    painter->drawImage(0, 0, image);

    if(showIndicator)
    {
        painter->restore();

        painter->translate(width() - 10, height() - 10);

        painter->rotate(indicatorAngle);

        QPainterPath path;
        path.addRect(-5, -5, 10, 10);

        painter->setRenderHint(QPainter::Antialiasing, rotateIndicator);

        painter->fillPath(path, QColor(0, 0, 0, 100));
        painter->strokePath(path, QPen(QColor(0, 0, 0, 200)));
    }
}

void GraphicsScene::increaseIndicatorAngle()
{
    if(rotateIndicator) indicatorAngle++;
}
