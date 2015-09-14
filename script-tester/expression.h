#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

class Node;
class Context;

class Expression
{
    friend class Parser;

    Node *root;
    QString msg;

public:
    Expression();
    ~Expression();

    bool isError();
    QString errorMsg();

    double eval(Context *context);
};

#endif // EXPRESSION_H
