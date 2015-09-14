#ifndef CELLENGINE_H
#define CELLENGINE_H

#include <QObject>
#include <QColor>
#include <QImage>
#include <QFutureSynchronizer>

class Expression;
class Context;

enum ColoringMode { BinaryColoring,
                    HslColoring,
                    MonochromeColoring };

enum Rule { BlankRule,
            LifeRule,
            DemonsRule,
            ScriptRule };

class CellEngine : public QObject
{
    Q_OBJECT

    uchar *data[2];
    QRgb *colormap;
    QImage *image[2];

    QFutureSynchronizer<void> sync;

    int width, height, mod, interval, affinity;
    ColoringMode coloringMode;
    Rule rule;

    bool abort, alive;

    Expression *script;
    QVector<Context *> contexts;
    QVector<uchar *> cells;
    QVector<int *> xs, ys;
    QVector<QString> errors;

public:
    CellEngine(QObject *parent = 0);
    ~CellEngine();

    void compileScript(QString code);

    const QImage &getImage();

    void setColoringMode(ColoringMode mode);
    void setRule(Rule rule);

    void setMod(int mod);
    void setInterval(int interval);
    void setAffinity(int affinity);

    void resize(int w, int h);

    void start();
    void stop();
    void toggle();

    void randomize();

    void render();

private slots:
    void run();

private:
    void doRandomize(int i);
    void doRender(int i);
    void doRun(int i);

    void createColormap();
};

#endif // CELLENGINE_H
