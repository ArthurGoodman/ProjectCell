#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

#include "syntaxhighlighter.h"
#include "scriptedit.h"
#include "kernel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(qApp->desktop()->size()/2);
    setMinimumSize(qApp->desktop()->size()/4);

    new SyntaxHighlighter(ui->frame->textEdit()->document());

    //    setStyleSheet("QMainWindow { background: rgb(80, 80, 80) }");
    //    ui->textEdit->setStyleSheet("QTextEdit { background: rgb(40, 40, 40) }");
    //    ui->textEdit->setFrameShape(QFrame::Box);
    //    ui->frame->setFrameShape(QFrame::Box);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::error(QString msg)
{
    cleanOutput();

    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->append(msg);
}

void MainWindow::puts(QString text)
{
    ui->textEdit->setTextColor(Qt::black);
    //    ui->textEdit->setTextColor(QColor(230, 230, 230));
    ui->textEdit->append(text);
}

void MainWindow::print(QString text)
{
    ui->textEdit->setTextColor(Qt::black);
    //    ui->textEdit->setTextColor(QColor(230, 230, 230));
    ui->textEdit->insertPlainText(text);
}

void MainWindow::toggleFullscreen()
{
    if(isFullScreen()) showNormal();
    else showFullScreen();
}

void MainWindow::loadScript()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Script", "", "Cell Script Files (*.cell)");
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) return;

    ui->frame->textEdit()->setText(file.readAll());

    file.close();
}

void MainWindow::saveScript()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Script", "", "Cell Script Files (*.cell)");
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) return;

    QTextStream stream(&file);
    stream << ui->frame->textEdit()->document()->toPlainText();

    file.close();
}

void MainWindow::cleanOutput()
{
    ui->textEdit->clear();
}

void MainWindow::actionAbout()
{
    QMessageBox::about(this, "About Project Cell Script Tester", "<b>Project Cell Script Tester v1.0</b> by Nikita Kartavov");
}

void MainWindow::runScript()
{
    Kernel::run(ui->frame->textEdit()->document()->toPlainText());
}
