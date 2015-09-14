#include <QApplication>

#include "ui.h"
#include "kernel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Kernel::initialize();
    Ui::initialize();

    return a.exec();
}
