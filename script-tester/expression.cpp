#include "expression.h"

#include "nodes.h"

Expression::Expression() :
    root(0)
{
}

Expression::~Expression()
{
    if(root) delete root;
}

bool Expression::isError()
{
    return !msg.isEmpty();
}

QString Expression::errorMsg()
{
    return msg;
}

double Expression::eval(Context *context)
{
    return root->eval(context);
}
