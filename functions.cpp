#include "functions.h"
#include <QtWidgets>

#include "context.h"
#include "nodes.h"

Function::Function(int arity) :
    ar(arity)
{
}

Function::~Function()
{
}

int Function::arity()
{
    return ar;
}

InterpretedFunction::InterpretedFunction(int arity, Context *context, Node *body) :
    Function(arity), context(context), body(body)
{
}

InterpretedFunction::~InterpretedFunction()
{
//    delete context;
//    delete body;
}

NaryInterpretedFunction::NaryInterpretedFunction(QStringList parameters, Context *context, Node *body) :
    InterpretedFunction(parameters.size(), context, body), parameters(parameters)
{
}

double NaryInterpretedFunction::call(QList<double> arguments)
{
    Context *context = this->context->createChildContext();

    QList<double>::Iterator i = arguments.begin();

    foreach(QString param, parameters)
        context->setLocalVariable(param, *(i++), true);

    double value = body->eval(context);

    delete context;

    return value;
}


NullaryInterpretedFunction::NullaryInterpretedFunction(Context *context, Node *body) :
    InterpretedFunction(0, context, body)
{
}

double NullaryInterpretedFunction::call(QList<double>)
{
    return body->eval(context->createChildContext());
}
