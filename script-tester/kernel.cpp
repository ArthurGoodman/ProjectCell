#include "kernel.h"
#include <QtWidgets>

#include "ui.h"
#include "mainwindow.h"
#include "parser.h"
#include "expression.h"
#include "context.h"
#include "exceptions.h"
#include "functions.h"

Parser *Kernel::parser;

void Kernel::initialize()
{
    qsrand(QTime().msecsTo(QTime::currentTime()));

    parser = new Parser;
}

void Kernel::run(QString script)
{
    Expression *expr = parser->compile(script);

    Context *context = new Context;

    class PrintFunction : public Function
    {
    public:
        PrintFunction() : Function(1) {}

        double call(QList<double> arguments)
        {
            UI::mainWindow->print(QString::number(arguments.first()));
            return 0;
        }
    };

    context->setCoreFunction("print", new PrintFunction);

    class PutsFunction : public Function
    {
    public:
        PutsFunction() : Function(1) {}

        double call(QList<double> arguments)
        {
            UI::mainWindow->puts(QString::number(arguments.first()));
            return 0;
        }
    };

    context->setCoreFunction("puts", new PutsFunction);

    class NewlineFunction : public Function
    {
    public:
        NewlineFunction() : Function(0) {}

        double call(QList<double>)
        {
            UI::mainWindow->print("\n");
            return 0;
        }
    };

    context->setCoreFunction("newline", new NewlineFunction);

    class SpaceFunction : public Function
    {
    public:
        SpaceFunction() : Function(0) {}

        double call(QList<double>)
        {
            UI::mainWindow->print(" ");
            return 0;
        }
    };

    context->setCoreFunction("space", new SpaceFunction);

    class RandFunction : public Function
    {
    public:
        RandFunction() : Function(0) {}

        double call(QList<double>)
        {
            return (double)qrand()/RAND_MAX;
        }
    };

    context->setCoreFunction("rand", new RandFunction);

    if(expr->isError()) { UI::mainWindow->error(expr->errorMsg()); }
    else
    {
        try
        {
            UI::mainWindow->cleanOutput();
            UI::mainWindow->print("\n=> " + QString::number(expr->eval(context)));
        }
        catch(ErrorException *e) { UI::mainWindow->error(e->message()); delete e; }
        catch(BreakException *e) { UI::mainWindow->error("'break' not within a loop"); delete e; }
        catch(ReturnException *e) { UI::mainWindow->error("'return' not within a function"); delete e; }
        catch(ContinueException *e) { UI::mainWindow->error("'continue' not within a loop"); delete e; }
    }

    delete context;
}
