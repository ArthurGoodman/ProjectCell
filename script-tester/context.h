#ifndef CONTEXT_H
#define CONTEXT_H

#include <QMap>
#include <QString>

class Function;
class CoreFunction;

class Context
{
    Context *parent;

    QMap<QString, double> variables;
    QMap<QString, Function *> *coreFunctions, functions;

public:
    Context(Context *parent = 0);
    ~Context();

    Context *createChildContext();

    void clear();

    void setLocalVariable(const QString &name, const double &value, bool check = false);
    bool hasLocalVariable(const QString &name);
    double getLocalVariable(const QString &name);

    void setVariable(const QString &name, const double &value);
    bool hasVariable(const QString &name);
    double getVariable(const QString &name);

    void setCoreFunction(const QString &name, Function *f);
    bool hasCoreFunction(const QString &name);
    Function *getCoreFunction(const QString &name);

    void setLocalFunction(const QString &name, Function *f, bool check = false);
    bool hasLocalFunction(const QString &name);
    Function *getLocalFunction(const QString &name);

    bool hasFunction(const QString &name);
    Function *getFunction(const QString &name);
};

#endif // CONTEXT_H
