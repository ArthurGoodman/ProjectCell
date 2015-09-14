#include "mainwindow.h"
#include <QtWidgets>

#include "ui.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "console.h"
#include "kernel.h"
#include "cellengine.h"
#include "scriptedit.h"

MainWindow::MainWindow()
{
    consoleVisible = false;
    scriptEditVisible = false;
    alive = true;
}

void MainWindow::toggleConsole()
{
    QPropertyAnimation *a = new QPropertyAnimation(Ui::console, "geometry");
    connect(a, SIGNAL(finished()), SLOT(resizeConsole()));
    a->setEasingCurve(QEasingCurve::InOutSine);
    a->setDuration(500);
    a->setStartValue(Ui::console->rect());

    if(consoleVisible)
    {
        a->setEndValue(QRect(0, 0, width(), 0));
        Ui::console->clearFocus();
    }
    else
    {
        a->setEndValue(QRect(0, 0, width(), height()/3));
        Ui::console->setFocus();
    }

    a->start();

    consoleVisible = !consoleVisible;
}

void MainWindow::toggleScriptEdit()
{
    if(scriptEditVisible) { Ui::scriptEdit->hide(); Ui::scriptEdit->textEdit()->clearFocus(); }
    else { Ui::scriptEdit->show(); Ui::scriptEdit->textEdit()->setFocus(); }

    scriptEditVisible = !scriptEditVisible;
}

bool MainWindow::isConsoleVisible()
{
    return consoleVisible;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    Ui::view->scene()->setSceneRect(rect());

    resizeConsole();
    resizeScriptEdit();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_F11: isFullScreen() ? showNormal() : showFullScreen(); break;

    case Qt::Key_Escape:
        if(consoleVisible) toggleConsole();
        else if(scriptEditVisible) toggleScriptEdit();
        else isFullScreen() ? showNormal() : (void)close(); break;

    case Qt::Key_AsciiTilde: case Qt::Key_QuoteLeft: if(scriptEditVisible) toggleScriptEdit(); toggleConsole(); break;

    case Qt::Key_Space: if(e->modifiers() & Qt::ControlModifier)
        {
            if(scriptEditVisible)
            {
                Kernel::engine->compileScript(Ui::scriptEdit->textEdit()->document()->toPlainText());
                toggleScriptEdit();
            }
            else { if(consoleVisible) toggleConsole(); toggleScriptEdit(); }
        }
        else if(!Ui::console->hasFocus() && !Ui::scriptEdit->isVisible())
        {
            Kernel::engine->toggle();
            Ui::scene->toggleIndicatorRotation();
        }
        break;

    case Qt::Key_R: Kernel::engine->randomize(); break;
        //    case Qt::Key_Space: if(alive) Kernel::engine->stop(); else Kernel::engine->start(); alive = !alive; break;
    }
}

void MainWindow::resizeConsole()
{
    Ui::console->resize(width(), consoleVisible ? height()/3 : 0);
}

void MainWindow::resizeScriptEdit()
{
    Ui::scriptEdit->resize(size());
}
