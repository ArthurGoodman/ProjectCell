#include "cellengine.h"
#include <QtWidgets>
#include <QtConcurrent>

#include "expression.h"
#include "kernel.h"
#include "ui.h"
#include "parser.h"
#include "console.h"
#include "mainwindow.h"
#include "context.h"
#include "exceptions.h"
#include "functions.h"
#include "graphicsscene.h"

CellEngine::CellEngine(QObject *parent) :
    QObject(parent)
{
    width = height = 1000; mod = 2; interval = 16;
    coloringMode = BinaryColoring;
    colormap = 0;

    createColormap();

    data[0] = new uchar[width*height]; data[1] = new uchar[width*height];
    image[0] = new QImage(width, height, QImage::Format_ARGB32_Premultiplied); image[1] = new QImage(width, height, QImage::Format_ARGB32_Premultiplied);

    abort = false;
    alive = false;

    setAffinity(QThread::idealThreadCount());

    rule = ScriptRule;
    script = 0;
    compileScript("");
    rule = BlankRule;
}

CellEngine::~CellEngine()
{
    delete [] data[0];
    delete [] data[1];

    delete [] colormap;
}

void CellEngine::compileScript(QString code)
{
    if(rule != ScriptRule) return;

    bool wasAlive = alive;
    if(alive) stop();

    Expression *expr = Kernel::parser->compile(code);

    if(expr->isError())
    {
        *Ui::console << expr->errorMsg() << ""; Ui::mainWindow->toggleConsole();
        Ui::console->insertBlock(); Ui::console->insertPrompt();
    }
    else { Expression *old = script; script = expr; if(old) delete old; }

    if(wasAlive) start();
}

const QImage &CellEngine::getImage()
{
    return *image[0];
}

void CellEngine::setColoringMode(ColoringMode mode)
{
    bool wasAlive = alive;
    if(alive) stop();

    coloringMode = mode;
    createColormap();

    if(wasAlive) start();
}

void CellEngine::setRule(Rule rule)
{
    bool wasAlive = alive;
    if(alive) stop();

    this->rule = rule;

    if(wasAlive) start();
}

void CellEngine::setMod(int mod)
{
    if(mod < 2 || mod > 256) return;

    bool wasAlive = alive;
    if(alive) stop();

    delete [] colormap;
    this->mod = mod;
    createColormap();

    render();

    if(wasAlive) start();
}

void CellEngine::setInterval(int interval)
{
    if(interval < 0 || interval > 1000) return;
    this->interval = interval;
}

void CellEngine::setAffinity(int affinity)
{
    if(affinity < 1 || affinity > QThread::idealThreadCount()) return;

    bool wasAlive = alive;
    if(alive) stop();

    this->affinity = affinity;

    foreach(Context *context, contexts) delete context;
    foreach(uchar *cell, cells) delete cell;
    foreach(int *x, xs) delete x;
    foreach(int *y, ys) delete y;

    contexts.clear();
    cells.clear();
    xs.clear();
    ys.clear();
    errors.clear();

    for(int i = 0; i < affinity; ++i)
    {
        contexts << new Context;
        cells << new uchar;
        xs << new int;
        ys << new int;
        errors << "";

        class CellFunction : public Function
        {
            uchar &cell;

        public:
            CellFunction(uchar &cell) : Function(0), cell(cell) {}
            double call(QList<double>) { return cell; }
        };

        contexts.last()->setCoreFunction("cell", new CellFunction(*cells.last()));

        class XFunction : public Function
        {
            int &x;

        public:
            XFunction(int &x) : Function(0), x(x) {}
            double call(QList<double>) { return x; }
        };

        contexts.last()->setCoreFunction("x", new XFunction(*xs.last()));

        class YFunction : public Function
        {
            int &y;

        public:
            YFunction(int &y) : Function(0), y(y) {}
            double call(QList<double>) { return y; }
        };

        contexts.last()->setCoreFunction("y", new YFunction(*ys.last()));

        class AtFunction : public Function
        {
            uchar *&data; int &width, &height;

        public:
            AtFunction(uchar *&data, int &width, int &height) : Function(2), data(data), width(width), height(height) {}

            double call(QList<double> arguments)
            {
                int x = arguments[0], y = arguments[1];
                if(x < 0) x = -x % width; else if(x >= width) x = width - 1 - x % width;
                if(y < 0) y = -y % height; else if(y >= height) y = height - 1 - y % height;
                return data[y*width+x];
            }
        };

        contexts.last()->setCoreFunction("at", new AtFunction(data[0], width, height));

        class RandFunction : public Function
        {
        public:
            RandFunction() : Function(0) {}
            double call(QList<double>) { return (double)qrand()/RAND_MAX; }
        };

        contexts.last()->setCoreFunction("rand", new RandFunction());

        class ModFunction : public Function
        {
            int &mod;

        public:
            ModFunction(int &mod) : Function(0), mod(mod) {}
            double call(QList<double>) { return mod; }
        };

        contexts.last()->setCoreFunction("mod", new ModFunction(mod));

        class NeumannFunction : public Function
        {
            uchar *&data; int &width, &height, &x, &y;

        public:
            NeumannFunction(uchar *&data, int &width, int &height, int &x, int &y) : Function(0), data(data), width(width), height(height), x(x), y(y) {}

            double call(QList<double>)
            {
                return  data[y*width+(x==0?width-1:x-1)] + data[y*width+(x==width-1?0:x+1)] +
                        data[(y==0?height-1:y-1)*width+x] + data[(y==height-1?0:y+1)*width+x];
            }
        };

        contexts.last()->setCoreFunction("neumann", new NeumannFunction(data[0], width, height, *xs.last(), *ys.last()));

        class MooreFunction : public Function
        {
            uchar *&data; int &width, &height, &x, &y;

        public:
            MooreFunction(uchar *&data, int &width, int &height, int &x, int &y) : Function(0), data(data), width(width), height(height), x(x), y(y) {}

            double call(QList<double>)
            {
                return  data[y*width+(x==0?width-1:x-1)] + data[y*width+(x==width-1?0:x+1)] +
                        data[(y==0?height-1:y-1)*width+x] + data[(y==height-1?0:y+1)*width+x] +
                        data[(y==0?height-1:y-1)*width+(x==0?width-1:x-1)] + data[(y==height-1?0:y+1)*width+(x==width-1?0:x+1)] +
                        data[(y==0?height-1:y-1)*width+(x==width-1?0:x+1)] + data[(y==height-1?0:y+1)*width+(x==0?width-1:x-1)];
            }
        };

        contexts.last()->setCoreFunction("moore", new MooreFunction(data[0], width, height, *xs.last(), *ys.last()));
    }

    if(wasAlive) start();
}

void CellEngine::resize(int w, int h)
{
    if(w < 1 || h < 1 || w > 2000 || h > 2000) return;

    bool wasAlive = alive;
    if(alive) stop();

    uchar *a = new uchar[w*h], *b = new uchar[w*h];

    delete image[0]; delete image[1];
    image[0] = new QImage(w, h, QImage::Format_ARGB32_Premultiplied); image[1] = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    QRgb *bits[2] = { (QRgb *)image[0]->bits(), (QRgb *)image[1]->bits() };

    for(int y = 0; y < h; ++y) for(int x = 0; x < w; ++x)
    {
        int pos = y*w+x;
        *bits[0]++ = *bits[1]++ = colormap[a[pos] = b[pos] = x < width && y < height ? data[0][y*width+x] : 0];
    }

    delete [] data[0]; delete [] data[1];
    data[0] = a; data[1] = b;

    width = w;
    height = h;

    if(wasAlive) start();
}

void CellEngine::start()
{
    abort = false;
    run();
}

void CellEngine::stop()
{
    abort = true;
    while(alive) qApp->processEvents();
}

void CellEngine::toggle()
{
    if(alive) stop();
    else start();
}

void CellEngine::randomize()
{
    bool wasAlive = alive;
    if(alive) stop();

    QFutureSynchronizer<void> sync;
    for(int c = 0; c < affinity; ++c) sync.addFuture(QtConcurrent::run(this, &CellEngine::doRandomize, c));
    sync.waitForFinished();

    if(wasAlive) start();
}

void CellEngine::render()
{
    QFutureSynchronizer<void> sync;
    for(int c = 0; c < affinity; ++c) sync.addFuture(QtConcurrent::run(this, &CellEngine::doRender, c));
    sync.waitForFinished();
}

void CellEngine::run()
{
    if(abort) { alive = false; return; }
    alive = true;

    for(int i = 0; i < affinity; ++i)
        errors[i] = "";

    sync.clearFutures();
    for(int c = 0; c < affinity; ++c) sync.addFuture(QtConcurrent::run(this, &CellEngine::doRun, c));
    sync.waitForFinished();

    for(int i = 0; i < affinity; ++i)
    {
        if(errors[i].isEmpty()) continue;
        *Ui::console << errors[i] + " [thread " + QString::number(i) + "]\n";
        Ui::console->insertBlock(); Ui::console->insertPrompt();
        if(!Ui::mainWindow->isConsoleVisible()) Ui::mainWindow->toggleConsole();
        alive = false; stop(); Ui::scene->toggleIndicatorRotation(); return;
    }

    uchar *temp = data[0]; data[0] = data[1]; data[1] = temp;
    QImage *img = image[0]; image[0] = image[1]; image[1] = img;

    QTimer::singleShot(interval, this, SLOT(run()));
}

void CellEngine::doRandomize(int i)
{
    qsrand(QTime::currentTime().msec()*(i+1));

    uchar *a = data[0], *b = data[1];
    int p = height*double(i)/affinity, q = height*double(i+1)/affinity;
    QRgb *bits[2] = { (QRgb *)image[0]->bits() + p*width, (QRgb *)image[1]->bits() + p*width };

    for(int y = p; y < q; ++y) for(int x = 0; x < width; ++x)
    {
        int pos = y*width+x;
        *bits[0]++ = *bits[1]++ = colormap[a[pos] = b[pos] = qrand() % mod];
    }
}

void CellEngine::doRender(int i)
{
    uchar *a = data[0];
    int p = height*double(i)/affinity, q = height*double(i+1)/affinity;
    QRgb *bits = (QRgb *)image[0]->bits() + p*width;

    for(int y = p; y < q; ++y) for(int x = 0; x < width; ++x)
        *bits++ = colormap[a[y*width+x] % mod];
}

void CellEngine::doRun(int i)
{
    uchar *a = data[0], *b = data[1];
    int p = height*double(i)/affinity, q = height*double(i+1)/affinity;
    QRgb *bits = (QRgb *)image[1]->bits() + p*width;

    Context *context = contexts[i];

    int &x = *(xs[i]), &y = *(ys[i]);

    for(y = p; y < q; ++y) for(x = 0; x < width; ++x)
    {
        context->clear();

        int pos = y*width+x;
        *(cells[i]) = a[pos];

        switch(rule)
        {
        case BlankRule:
            *bits++ = colormap[b[pos] = 0];
            break;

        case LifeRule:
        {
            uint sum =
                    a[width*y + (x==0?width-1:x-1)] + a[width*y + (x==width-1?0:x+1)] +
                    a[width*(y==0?height-1:y-1) + x] + a[width*(y==height-1?0:y+1) + x] +
                    a[width*(y==0?height-1:y-1) + (x==0?width-1:x-1)] + a[width*(y==height-1?0:y+1) + (x==0?width-1:x-1)] +
                    a[width*(y==0?height-1:y-1) + (x==width-1?0:x+1)] + a[width*(y==height-1?0:y+1) + (x==width-1?0:x+1)];

            if(sum == 3) b[pos] = 1; else if(a[pos] == 1 && sum != 2) b[pos] = 0; else b[pos] = a[pos];

            *bits++ = colormap[b[pos]];
        }
            break;

        case DemonsRule:
        {
            uchar next = (a[pos]+1) % mod;

            if(a[width*y + (x==0?width-1:x-1)] == next ||
                    a[width*y + (x==width-1?0:x+1)] == next ||
                    a[width*(y==0?height-1:y-1) + x] == next ||
                    a[width*(y==height-1?0:y+1) + x] == next) b[pos] = next;
            else b[pos] = a[pos];

            *bits++ = colormap[b[pos]];
        }
            break;

        case ScriptRule:
        {
            try
            {
                *bits++ = colormap[b[pos] = (uchar)script->eval(context) % mod];
            }
            catch(ErrorException *e)
            {
                errors[i] = e->message();
                delete e; return;
            }
            catch(BreakException *e)
            {
                errors[i] = "break not within a loop";
                delete e; return;
            }
            catch(ContinueException *e)
            {
                errors[i] = "continue not within a loop";
                delete e; return;
            }
            catch(ReturnException *e)
            {
                errors[i] = "return not within a function";
                delete e; return;
            }
        }
            break;
        }
    }
}

void CellEngine::createColormap()
{
    if(colormap) delete [] colormap;
    colormap = new QRgb[mod];

    switch(coloringMode)
    {
    case BinaryColoring: for(int i = 0; i < mod; ++i) colormap[i] = i ? qRgb(230, 230, 230) : qRgb(40, 40, 40); break;
    case HslColoring: for(int i = 0; i < mod; ++i) colormap[i] = QColor::fromHsv((double)i/(mod-1)*300, 255, 255).rgb(); break;
    case MonochromeColoring: for(int i = 0; i < mod; ++i) colormap[i] = QColor((double)i/(mod-1)*255, (double)i/(mod-1)*255, (double)i/(mod-1)*255).rgb(); break;
    }
}
