#include "mainwindow.h"
#include <QApplication>

#include "kernel.h"
#include "ui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Kernel::initialize();
    UI::initialize();

    return a.exec();
}
