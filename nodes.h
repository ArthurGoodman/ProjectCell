#ifndef NODES_H
#define NODES_H

#include <QList>
#include <QString>
#include <QStringList>

class Context;

class Node
{
public:
    virtual ~Node();
    virtual double eval(Context *context) = 0;
};

class BlockNode : public Node
{
    QList<Node *> nodes;
    bool createContext;

public:
    BlockNode(QList<Node *> nodes = QList<Node *>(), bool createContext = false);
    ~BlockNode();

    void addNode(Node *n);
    QList<Node *> getNodes();

    double eval(Context *context);
};

class BinaryNode : public Node
{
protected:
    Node *left, *right;

public:
    BinaryNode(Node *left, Node *right);
    virtual ~BinaryNode();
};

class PlusNode : public BinaryNode
{
public:
    PlusNode(Node *left, Node *right);
    double eval(Context *context);
};

class MinusNode : public BinaryNode
{
public:
    MinusNode(Node *left, Node *right);
    double eval(Context *context);
};

class MultiplyNode : public BinaryNode
{
public:
    MultiplyNode(Node *left, Node *right);
    double eval(Context *context);
};

class DivideNode : public BinaryNode
{
public:
    DivideNode(Node *left, Node *right);
    double eval(Context *context);
};

class ModNode : public BinaryNode
{
public:
    ModNode(Node *left, Node *right);
    double eval(Context *context);
};

class PowerNode : public BinaryNode
{
public:
    PowerNode(Node *left, Node *right);
    double eval(Context *context);
};

class LtNode : public BinaryNode
{
public:
    LtNode(Node *left, Node *right);
    double eval(Context *context);
};

class GtNode : public BinaryNode
{
public:
    GtNode(Node *left, Node *right);
    double eval(Context *context);
};

class LeNode : public BinaryNode
{
public:
    LeNode(Node *left, Node *right);
    double eval(Context *context);
};

class GeNode : public BinaryNode
{
public:
    GeNode(Node *left, Node *right);
    double eval(Context *context);
};

class EqNode : public BinaryNode
{
public:
    EqNode(Node *left, Node *right);
    double eval(Context *context);
};

class NeNode : public BinaryNode
{
public:
    NeNode(Node *left, Node *right);
    double eval(Context *context);
};

class AndNode : public BinaryNode
{
public:
    AndNode(Node *left, Node *right);
    double eval(Context *context);
};

class OrNode : public BinaryNode
{
public:
    OrNode(Node *left, Node *right);
    double eval(Context *context);
};

class XorNode : public BinaryNode
{
public:
    XorNode(Node *left, Node *right);
    double eval(Context *context);
};

class UnaryNode : public Node
{
protected:
    Node *node;

public:
    UnaryNode(Node *node);
    virtual ~UnaryNode();
};

class UnaryMinusNode : public UnaryNode
{
public:
    UnaryMinusNode(Node *node);
    double eval(Context *context);
};

class NotNode : public UnaryNode
{
public:
    NotNode(Node *node);
    double eval(Context *context);
};

class PrefDecNode : public Node
{
    QString name;

public:
    PrefDecNode(QString name);
    double eval(Context *context);
};

class PrefIncNode : public Node
{
    QString name;

public:
    PrefIncNode(QString name);
    double eval(Context *context);
};

class SuffDecNode : public Node
{
    QString name;

public:
    SuffDecNode(QString name);
    double eval(Context *context);
};

class SuffIncNode : public Node
{
    QString name;

public:
    SuffIncNode(QString name);
    double eval(Context *context);
};

class ValueNode : public Node
{
    double value;

public:
    ValueNode(double value);
    double eval(Context *context);
};

class IfNode : public Node
{
    Node *condition, *body, *elseBody;

public:
    IfNode(Node *condition, Node *body, Node *elseBody); ~IfNode();
    double eval(Context *context);
};

class WhileNode : public Node
{
    Node *condition, *body;

public:
    WhileNode(Node *condition, Node *body); ~WhileNode();
    double eval(Context *context);
};

class ForNode : public Node
{
    Node *init, *condition, *increment, *body;

public:
    ForNode(Node *init, Node *condition, Node *increment, Node *body); ~ForNode();
    double eval(Context *context);
};

class VariableDefinitionNode : public Node
{
    QString name; Node *expr;

public:
    VariableDefinitionNode(QString name, Node *expr = 0);
    double eval(Context *context);
};

class VariableNode : public Node
{
    QString name; Node *expr;

public:
    VariableNode(QString name, Node *expr = 0);
    double eval(Context *context);

    QString getName();
};

class FunctionDefinitionNode : public Node
{
    QString name; QStringList parameters; Node *body;

public:
    FunctionDefinitionNode(QString name, QStringList parameters, Node *body);
    double eval(Context *context);
};

class CallNode : public Node
{
    QString name;
    QList<Node *> arguments;

public:
    CallNode(QString name, QList<Node *> arguments);
    double eval(Context *context);
};

class BreakNode : public Node
{
    Node *value;

public:
    BreakNode(Node *value = 0); ~BreakNode();
    double eval(Context *context);
};

class ReturnNode : public Node
{
    Node *value;

public:
    ReturnNode(Node *value = 0); ~ReturnNode();
    double eval(Context *context);
};

class ContinueNode : public Node
{
public:
    double eval(Context *context);
};

#endif // NODES_H
