#include "consoleparser.h"
#include <QtWidgets>

#include "console.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "scriptedit.h"

#include "ui.h"
#include "kernel.h"
#include "cellengine.h"
#include "mainwindow.h"

ConsoleParser::ConsoleParser(Console *console) :
    console(console)
{
    connect(console, SIGNAL(command(QString)), SLOT(command(QString)));
}

bool ConsoleParser::execute(QString cmd)
{
    this->cmd = cmd; pos = 0;

    if(getToken().isEmpty()) {}
    else if(token == "cls" || token == "clear") { console->clear(); console->insertPrompt(); return false; }
    else if(token == "exit" || token == "quit") qApp->quit();
    else if(token == "help" || token == "?")
    {
        *console << "Project Cell v2.1 by Arthur Goodman"
                 << ""
                 << "Commands:"
                 << " cls | clear \t\t\t\t\t Clear console."
                 << " exit | quit \t\t\t\t\t Close the program."
                 << " help | ? \t\t\t\t\t Show help information."
                 << ""
                 << " rand \t\t\t\t\t\t Fill the Field with random values."
                 << " resize <number : 1..2000> <number : 1..2000> \t\t Change the size of the field."
                 << ""
                 << " script \t\t\t\t\t\t Launch script editor."
                 << ""
                 << " mod <number : 2..256> \t\t\t\t Set the maximum value of the cell."
                 << " int <number : 0..1000> \t\t\t\t Set the update interval."
                 << " color <coloring_mode> \t\t\t\t Set the coloring mode."
                 << "   Available <coloring_mode>s are:"
                 << "    bin \t\t\t\t\t\t Binary."
                 << "    hsl \t\t\t\t\t\t HSL palette."
                 << "    mono \t\t\t\t\t\t Monochrome."
                 << ""
                 << " indicator \t\t\t\t\t Toggle the spinning indicator."
                 << " affinity <number : 1..number of logical processors> \t Set the number of threads."
                 << ""
                 << " rule <rule>"
                 << "   Available <rule>s are:"
                 << "    blank \t\t\t\t\t Zeroes."
                 << "    life \t\t\t\t\t\t Conway's Game of Life."
                 << "    demons \t\t\t\t\t Demons are everywhere!"
                 << "    script \t\t\t\t\t User's script."
                 << "";
    }

    else if(token == "rand") Kernel::engine->randomize();
    else if(token == "resize") Kernel::engine->resize(getToken().toInt(), getToken().toInt());

    else if(token == "script")
    {
        Ui::mainWindow->toggleConsole();
        Ui::mainWindow->toggleScriptEdit();
    }

    else if(token == "load")
    {
        QFile file(QDir::currentPath() + "/scripts/" + getToken() + ".cell");
        if(!file.open(QFile::ReadOnly | QFile::Text)) *console << "error opening file '" + file.fileName() + "'\n";
        else Ui::scriptEdit->textEdit()->setText(file.readAll());

        Kernel::engine->compileScript(Ui::scriptEdit->textEdit()->document()->toPlainText());
    }
    else if(token == "save")
    {
        QFile file(QDir::currentPath() + "/scripts/" + getToken() + ".cell");
        if(!file.open(QFile::WriteOnly | QFile::Text)) *console << "error opening file '" + file.fileName() + "'\n";
        else QTextStream(&file) << Ui::scriptEdit->textEdit()->document()->toPlainText();
    }

    else if(token == "mod") Kernel::engine->setMod(getToken().toInt());
    else if(token == "int") Kernel::engine->setInterval(getToken().toInt());
    else if(token == "color")
    {
        if(getToken() == "bin") Kernel::engine->setColoringMode(BinaryColoring);
        else if(token == "hsl") Kernel::engine->setColoringMode(HslColoring);
        else if(token == "mono") Kernel::engine->setColoringMode(MonochromeColoring);
        else *console << "error: invalid coloring mode\n";

        Kernel::engine->render();
    }

    else if(token == "indicator") Ui::scene->toggleIndicator();
    else if(token == "affinity") Kernel::engine->setAffinity(getToken().toInt());
    else if(token == "rule")
    {
        if(getToken() == "blank") Kernel::engine->setRule(BlankRule);
        else if(token == "life") Kernel::engine->setRule(LifeRule);
        else if(token == "demons") Kernel::engine->setRule(DemonsRule);
        else if(token == "script") Kernel::engine->setRule(ScriptRule);
        else *console << "error: invalid rule\n";
    }

    else *console << "error: unknown command '" + token + "'\n";

    return true;
}

QString ConsoleParser::getToken()
{
    token = "";

    while(cmd[pos].isSpace() || cmd[pos] == '\n') pos++;

    if(cmd[pos] == '"') while(cmd[++pos] != '"' && !cmd[pos].isNull()) token += cmd[pos];
    else while(!cmd[pos].isSpace() && !cmd[pos].isNull()) token += cmd[pos++];

    return token;
}

void ConsoleParser::command(QString cmd)
{
    console->lock();

    if(execute(cmd))
    {
        console->insertBlock();
        console->insertPrompt();
    }
}
