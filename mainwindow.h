#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

//    QMap<int, QString> bindins;

    bool consoleVisible, scriptEditVisible;
    bool alive;

public:
    MainWindow();

    void toggleConsole();
    void toggleScriptEdit();

//    void bind(int key, QString command);

    bool isConsoleVisible();

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void resizeConsole();
    void resizeScriptEdit();
};

#endif // MAINWINDOW_H
