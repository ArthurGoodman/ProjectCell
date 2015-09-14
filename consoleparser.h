#ifndef CONSOLEPARSER_H
#define CONSOLEPARSER_H

#include <QObject>

class Console;

class ConsoleParser : public QObject
{
    Q_OBJECT

    Console *console;
    QString cmd, token;
    int pos;

public:
    ConsoleParser(Console *console);

    bool execute(QString cmd);

private:
    QString getToken();

public slots:
    void command(QString cmd);
};

#endif // CONSOLEPARSER_H
