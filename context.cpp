#include "context.h"

#include "functions.h"
#include "exceptions.h"

Context::Context(Context *parent) : parent(parent)
{
    if(!parent) coreFunctions = new QMap<QString, Function *>;
    else coreFunctions = 0;
}

Context::~Context()
{
//    foreach(Function *f, *coreFunctions) delete f;
    if(coreFunctions) delete coreFunctions;

    foreach(Function *f, functions) delete f;
}

Context *Context::createChildContext() { return new Context(this); }

void Context::clear()
{
    foreach(Function *f, functions) delete f;
    variables.clear(); functions.clear();
}

void Context::setLocalVariable(const QString &name, const double &value, bool check)
{
    if(check && variables.contains(name)) throw new RuntimeError("variable '" + name + "' is already defined");
    variables[name] = value;
}

bool Context::hasLocalVariable(const QString &name) { return variables.contains(name); }
double Context::getLocalVariable(const QString &name) { return variables.value(name); }

void Context::setVariable(const QString &name, const double &value)
{
    if(hasLocalVariable(name)) setLocalVariable(name, value);
    else if(parent) parent->setVariable(name, value);
    else throw new RuntimeError("undefined variable '" + name + "'");
}

bool Context::hasVariable(const QString &name) { return hasLocalVariable(name) || (parent && parent->hasVariable(name)); }

double Context::getVariable(const QString &name)
{
//    return hasLocalVariable(name) ? getLocalVariable(name) : parent ? parent->getVariable(name) : 0;

    if(hasLocalVariable(name)) return getLocalVariable(name);
    if(parent) return parent->getVariable(name);
    throw new RuntimeError("undefined variable '" + name + "'");
}

void Context::setCoreFunction(const QString &name, Function *f) { (*coreFunctions)[name] = f; }
bool Context::hasCoreFunction(const QString &name) { return coreFunctions->contains(name); }
Function *Context::getCoreFunction(const QString &name) { return hasCoreFunction(name) ? coreFunctions->value(name) : parent ? parent->getCoreFunction(name) : 0; }

void Context::setLocalFunction(const QString &name, Function *f, bool check)
{
    if(check && functions.contains(name)) throw new RuntimeError("function '" + name + "' is already defined");
    functions[name] = f;
}

bool Context::hasLocalFunction(const QString &name) { return functions.contains(name); }
Function *Context::getLocalFunction(const QString &name) { return hasLocalFunction(name) ? functions.value(name) : parent ? parent->getLocalFunction(name) : 0; }

bool Context::hasFunction(const QString &name)
{ return hasLocalFunction(name) || (parent && parent->hasLocalFunction(name)) || hasCoreFunction(name); }

Function *Context::getFunction(const QString &name)
{
//    return hasLocalFunction(name) ? getLocalFunction(name) : parent ? parent->getFunction(name) : hasCoreFunction(name) ? getCoreFunction(name) : 0;

    if(!parent && hasCoreFunction(name)) return getCoreFunction(name);
    if(hasLocalFunction(name)) return getLocalFunction(name);
    if(parent) return parent->getFunction(name);
    throw new RuntimeError("undefined function '" + name + "'");
}
