#include "nodes.h"
#include <QtWidgets>

#include "context.h"
#include "exceptions.h"
#include "functions.h"

Node::~Node() {}

BlockNode::BlockNode(QList<Node *> nodes, bool createContext) : nodes(nodes), createContext(createContext) {}
BlockNode::~BlockNode() { foreach(Node *n, nodes) delete n; }

void BlockNode::addNode(Node *n) { nodes << n; }
QList<Node *> BlockNode::getNodes() { return nodes; }

double BlockNode::eval(Context *context)
{
    if(createContext)
        context = context->createChildContext();

    double r = 0;

    foreach(Node *n, nodes)
        r = n->eval(context);

    if(createContext) delete context;

    return r;
}

BinaryNode::BinaryNode(Node *left, Node *right) : left(left), right(right) {}
BinaryNode::~BinaryNode() { delete left; delete right; }

PlusNode::PlusNode(Node *left, Node *right) : BinaryNode(left, right) {}

double PlusNode::eval(Context *context)
{
    return left->eval(context) + right->eval(context);
}

MinusNode::MinusNode(Node *left, Node *right) : BinaryNode(left, right) {}

double MinusNode::eval(Context *context)
{
    return left->eval(context) - right->eval(context);
}

MultiplyNode::MultiplyNode(Node *left, Node *right) : BinaryNode(left, right) {}

double MultiplyNode::eval(Context *context)
{
    return left->eval(context) * right->eval(context);
}

DivideNode::DivideNode(Node *left, Node *right) : BinaryNode(left, right) {}

double DivideNode::eval(Context *context)
{
    return left->eval(context) / right->eval(context);
}

ModNode::ModNode(Node *left, Node *right) : BinaryNode(left, right) {}

double ModNode::eval(Context *context)
{
    double v = right->eval(context);
    return (int)left->eval(context) % int(v ? v : 1);
}

PowerNode::PowerNode(Node *left, Node *right) : BinaryNode(left, right) {}

double PowerNode::eval(Context *context)
{
    return powf(left->eval(context), right->eval(context));
}

LtNode::LtNode(Node *left, Node *right) : BinaryNode(left, right) {}

double LtNode::eval(Context *context)
{
    return left->eval(context) < right->eval(context);
}

GtNode::GtNode(Node *left, Node *right) : BinaryNode(left, right) {}

double GtNode::eval(Context *context)
{
    return left->eval(context) > right->eval(context);
}

LeNode::LeNode(Node *left, Node *right) : BinaryNode(left, right) {}

double LeNode::eval(Context *context)
{
    return left->eval(context) <= right->eval(context);
}

GeNode::GeNode(Node *left, Node *right) : BinaryNode(left, right) {}

double GeNode::eval(Context *context)
{
    return left->eval(context) >= right->eval(context);
}

EqNode::EqNode(Node *left, Node *right) : BinaryNode(left, right) {}

double EqNode::eval(Context *context)
{
    return left->eval(context) == right->eval(context);
}

NeNode::NeNode(Node *left, Node *right) : BinaryNode(left, right) {}

double NeNode::eval(Context *context)
{
    return left->eval(context) != right->eval(context);
}

AndNode::AndNode(Node *left, Node *right) : BinaryNode(left, right) {}

double AndNode::eval(Context *context)
{
    return left->eval(context) && right->eval(context);
}

OrNode::OrNode(Node *left, Node *right) : BinaryNode(left, right) {}

double OrNode::eval(Context *context)
{
    return left->eval(context) || right->eval(context);
}

XorNode::XorNode(Node *left, Node *right) : BinaryNode(left, right) {}

double XorNode::eval(Context *context)
{
    return !left->eval(context) != !right->eval(context);
}

UnaryNode::UnaryNode(Node *node) : node(node) {}
UnaryNode::~UnaryNode() { delete node; }

UnaryMinusNode::UnaryMinusNode(Node *node) : UnaryNode(node) {}

double UnaryMinusNode::eval(Context *context)
{
    return -node->eval(context);
}

NotNode::NotNode(Node *node) : UnaryNode(node) {}

double NotNode::eval(Context *context)
{
    return !node->eval(context);
}

PrefDecNode::PrefDecNode(QString name) : name(name) {}

double PrefDecNode::eval(Context *context)
{
    if(!context->hasVariable(name))
        throw new RuntimeError("undefined identifier '" + name + "'");

    double value = context->getVariable(name);

    context->setVariable(name, value - 1);

    return value - 1;
}

PrefIncNode::PrefIncNode(QString name) : name(name) {}

double PrefIncNode::eval(Context *context)
{
    if(!context->hasVariable(name))
        throw new RuntimeError("undefined identifier '" + name + "'");

    double value = context->getVariable(name);

    context->setVariable(name, value + 1);

    return value + 1;
}

SuffDecNode::SuffDecNode(QString name) : name(name) {}

double SuffDecNode::eval(Context *context)
{
    if(!context->hasVariable(name))
        throw new RuntimeError("undefined identifier '" + name + "'");

    double value = context->getVariable(name);

    context->setVariable(name, value - 1);

    return value;
}

SuffIncNode::SuffIncNode(QString name) : name(name) {}

double SuffIncNode::eval(Context *context)
{
    if(!context->hasVariable(name))
        throw new RuntimeError("undefined identifier '" + name + "'");

    double value = context->getVariable(name);

    context->setVariable(name, value + 1);

    return value;
}

ValueNode::ValueNode(double value) : value(value) {}
double ValueNode::eval(Context *) { return value; }

IfNode::IfNode(Node *condition, Node *body, Node *elseBody) : condition(condition), body(body), elseBody(elseBody) {}
IfNode::~IfNode() { delete condition; delete body; if(elseBody) delete elseBody; }

double IfNode::eval(Context *context)
{
    context = context->createChildContext();

    return condition->eval(context) ? body->eval(context) : elseBody ? elseBody->eval(context) : 0;
}

WhileNode::WhileNode(Node *condition, Node *body) : condition(condition), body(body) {}
WhileNode::~WhileNode() { delete condition; delete body; }

double WhileNode::eval(Context *context)
{
    context = context->createChildContext();

    double r = 0;

    while(condition->eval(context)) try { r = body->eval(context); }
    catch(BreakException *e) { double value = e->value(); delete e; return value; }
    catch(ContinueException *e) { delete e; continue; }
    catch(Exception *e) { e->raise(); }

    return r;
}

ForNode::ForNode(Node *init, Node *condition, Node *increment, Node *body) : init(init), condition(condition), increment(increment), body(body) {}
ForNode::~ForNode() { delete init; delete body; delete condition; delete increment; }

double ForNode::eval(Context *context)
{
    context = context->createChildContext();

    init->eval(context);

    double r = 0;

    while(condition->eval(context)) try
    {
        r = body->eval(context);
        increment->eval(context);
    }
    catch(BreakException *e) { double value = e->value(); delete e; return value; }
    catch(ContinueException *e)
    {
        delete e;
        increment->eval(context);
    }
    catch(Exception *e) { e->raise(); }

    return r;
}

VariableDefinitionNode::VariableDefinitionNode(QString name, Node *expr) : name(name), expr(expr) {}

double VariableDefinitionNode::eval(Context *context)
{
    //    if(context->hasLocalVariable(name))
    //        throw new RuntimeError("variable '" + name + "' is already defined");

    double value = expr ? expr->eval(context) : 0;
    context->setLocalVariable(name, value, true);
    return value;
}

VariableNode::VariableNode(QString name, Node *expr) : name(name), expr(expr) {}

double VariableNode::eval(Context *context)
{
    //    if(!context->hasVariable(name))
    //        throw new RuntimeError("undefined variable '" + name + "'");

    double value;

    if(expr) { value = expr->eval(context); context->setVariable(name, value); }
    else value = context->getVariable(name);

    return value;
}

QString VariableNode::getName() { return name; }

FunctionDefinitionNode::FunctionDefinitionNode(QString name, QStringList parameters, Node *body) : name(name), parameters(parameters), body(body) {}

double FunctionDefinitionNode::eval(Context *context)
{
    if(parameters.size()) context->setLocalFunction(name, new NaryInterpretedFunction(parameters, context, body), true);
    else context->setLocalFunction(name, new NullaryInterpretedFunction(context, body), true);

    return 0;
}

CallNode::CallNode(QString name, QList<Node *> arguments) : name(name), arguments(arguments) {}

double CallNode::eval(Context *context)
{
    //    if(!context->hasFunction(name))
    //        throw new RuntimeError("undefined function '" + name + "'");

    Function *f = context->getFunction(name);

    if(f->arity() != arguments.size()) throw new RuntimeError(QString("wrong amount of arguments (%1/%2)").arg(arguments.size()).arg(f->arity()));

    QList<double> evaledArguments;
    foreach(Node *node, arguments) evaledArguments << node->eval(context);

    double value;

    try { value = f->call(evaledArguments); }
    catch(ReturnException *e) { value = e->value(); delete e; }

    return value;
}


BreakNode::BreakNode(Node *value) : value(value) {}
BreakNode::~BreakNode() { if(value) delete value; }

double BreakNode::eval(Context *context)
{
    throw new BreakException(value ? value->eval(context) : 0);
}

ReturnNode::ReturnNode(Node *value) : value(value) {}
ReturnNode::~ReturnNode() { if(value) delete value; }

double ReturnNode::eval(Context *context)
{
    throw new ReturnException(value ? value->eval(context) : 0);
}

double ContinueNode::eval(Context *)
{
    throw new ContinueException;
}
