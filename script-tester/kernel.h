#ifndef KERNEL_H
#define KERNEL_H

#include <QString>

class Parser;

class Kernel
{
public:
    static Parser *parser;

    static void initialize();

    static void run(QString script);
};

#endif // KERNEL_H
