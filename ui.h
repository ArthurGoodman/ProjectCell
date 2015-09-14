#ifndef UI_H
#define UI_H

class MainWindow;
class GraphicsView;
class GraphicsScene;
class Console;
class ConsoleParser;
class ScriptEdit;

class Ui
{
public:
    static MainWindow *mainWindow;
    static GraphicsView *view;
    static GraphicsScene *scene;
    static Console *console;
    static ConsoleParser *consoleParser;
    static ScriptEdit *scriptEdit;

    static void initialize();
};

#endif // UI_H
