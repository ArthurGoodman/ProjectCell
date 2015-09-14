#include "ui.h"
#include <QtWidgets>

#include "mainwindow.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "console.h"
#include "scriptedit.h"
#include "consoleparser.h"
#include "syntaxhighlighter.h"

MainWindow *Ui::mainWindow;
GraphicsView *Ui::view;
GraphicsScene *Ui::scene;
Console *Ui::console;
ConsoleParser *Ui::consoleParser;
ScriptEdit *Ui::scriptEdit;

void Ui::initialize()
{
    view = new GraphicsView;

    view->setViewport(new QWidget);
    view->setScene(scene = new GraphicsScene);

    view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    view->setFrameShape(QFrame::NoFrame);

    mainWindow = new MainWindow;

    mainWindow->setCentralWidget(view);
    view->setFocus();

    mainWindow->resize(qApp->desktop()->size()/2);
    mainWindow->setMinimumSize(qApp->desktop()->size()/4);
    mainWindow->setWindowTitle("Project Cell");

    console = new Console;
    console->setAttribute(Qt::WA_TranslucentBackground);
    console->setWindowOpacity(0.8);
    console->setPrompt("$ ");

    view->scene()->addWidget(console);

    consoleParser = new ConsoleParser(console);

    scriptEdit = new ScriptEdit;

    new SyntaxHighlighter(scriptEdit->textEdit()->document());

    view->scene()->addWidget(scriptEdit);
    scriptEdit->hide();

    view->run();
    mainWindow->show();
}
