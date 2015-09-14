#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QPoint offset;
    double scale;

    bool showIndicator, rotateIndicator;
    int indicatorAngle;

    QTimer *indicatorTimer;

public:
    GraphicsScene();

    void shift(QPoint delta);
    void zoom(double factor);

    void toggleIndicator();
    void toggleIndicatorRotation();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private slots:
    void increaseIndicatorAngle();
};

#endif // GRAPHICSSCENE_H
