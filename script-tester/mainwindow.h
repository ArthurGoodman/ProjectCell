#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void error(QString msg);
    void puts(QString text);
    void print(QString text);

private:
    Ui::MainWindow *ui;

public slots:
    void toggleFullscreen();

    void loadScript();
    void saveScript();

    void cleanOutput();

    void actionAbout();

    void runScript();
};

#endif // MAINWINDOW_H
