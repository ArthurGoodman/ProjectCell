#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>

class Exception
{
public:
    virtual ~Exception();
    virtual void raise() = 0;
};

class ErrorException : public Exception
{
protected:
    QString msg;

public:
    virtual QString message() = 0;
};

class SyntaxError : public ErrorException
{
public:
    SyntaxError(QString msg);
    QString message();
    void raise();
};

class RuntimeError : public ErrorException
{
public:
    RuntimeError(QString msg);
    QString message();
    void raise();
};

class ValueException : public Exception
{
protected:
    double v;

public:
    double value();
};

class BreakException : public ValueException
{
public:
    BreakException(double value);
    void raise();
};

class ContinueException: public Exception
{
public:
    void raise();
};

class ReturnException : public ValueException
{
public:
    ReturnException(double value);
    void raise();
};

#endif // EXCEPTIONS_H
