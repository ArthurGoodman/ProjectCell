#ifndef KERNEL_H
#define KERNEL_H

class CellEngine;
class Parser;

class Kernel
{
public:
    static CellEngine *engine;
    static Parser *parser;

    static void initialize();
};

#endif // KERNEL_H
