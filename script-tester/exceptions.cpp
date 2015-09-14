#include "exceptions.h"

Exception::~Exception()
{
}

SyntaxError::SyntaxError(QString msg)
{
    this->msg = msg;
}

QString SyntaxError::message()
{
    return "syntax error: " + msg;
}

void SyntaxError::raise()
{
    throw this;
}

RuntimeError::RuntimeError(QString msg)
{
    this->msg = msg;
}

QString RuntimeError::message()
{
    return "runtime error: " + msg;
}

void RuntimeError::raise()
{
    throw this;
}

double ValueException::value()
{
    return v;
}

BreakException::BreakException(double value)
{
    v = value;
}

void BreakException::raise()
{
    throw this;
}

void ContinueException::raise()
{
    throw this;
}

ReturnException::ReturnException(double value)
{
    v = value;
}

void ReturnException::raise()
{
    throw this;
}
