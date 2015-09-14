#include "ui.h"

#include "mainwindow.h"

MainWindow *UI::mainWindow;

void UI::initialize()
{
    mainWindow = new MainWindow;
    mainWindow->show();
}
