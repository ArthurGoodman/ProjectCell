#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QList>
#include <QString>
#include <QStringList>

class Context;
class Node;

class Function
{
    int ar;

public:
    Function(int arity);
    virtual ~Function();

    int arity();

    virtual double call(QList<double> arguments) = 0;
};

class InterpretedFunction : public Function
{
protected:
    Context *context;
    Node *body;

public:
    InterpretedFunction(int arity, Context *context, Node *body);
};

class NaryInterpretedFunction : public InterpretedFunction
{
    QStringList parameters;

public:
    NaryInterpretedFunction(QStringList parameters, Context *context, Node *body);
    double call(QList<double> arguments);
};

class NullaryInterpretedFunction : public InterpretedFunction
{
public:
    NullaryInterpretedFunction(Context *context, Node *body);
    double call(QList<double> arguments);
};

#endif // FUNCTIONS_H
