#include "kernel.h"
#include <QtWidgets>

#include "cellengine.h"
#include "parser.h"

CellEngine *Kernel::engine;
Parser *Kernel::parser;

void Kernel::initialize()
{
    qsrand(QTime().msecsTo(QTime::currentTime()));

    parser = new Parser;

    engine = new CellEngine;

    QThread *thread = new QThread;
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), engine, SLOT(deleteLater()));
    engine->moveToThread(thread);
    thread->start(QThread::HighestPriority);
}
