/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "scriptedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionQuit;
    QAction *actionLoad_Script;
    QAction *actionSave_Script;
    QAction *actionClear_Output;
    QAction *actionFullscreen;
    QAction *actionRun;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    ScriptEdit *frame;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(718, 511);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLoad_Script = new QAction(MainWindow);
        actionLoad_Script->setObjectName(QStringLiteral("actionLoad_Script"));
        actionSave_Script = new QAction(MainWindow);
        actionSave_Script->setObjectName(QStringLiteral("actionSave_Script"));
        actionClear_Output = new QAction(MainWindow);
        actionClear_Output->setObjectName(QStringLiteral("actionClear_Output"));
        actionFullscreen = new QAction(MainWindow);
        actionFullscreen->setObjectName(QStringLiteral("actionFullscreen"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame = new ScriptEdit(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frame, 0, 0, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(10);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 718, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionRun);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Script);
        menuFile->addAction(actionLoad_Script);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuEdit->addAction(actionClear_Output);
        menuView->addAction(actionFullscreen);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionFullscreen, SIGNAL(triggered()), MainWindow, SLOT(toggleFullscreen()));
        QObject::connect(actionLoad_Script, SIGNAL(triggered()), MainWindow, SLOT(loadScript()));
        QObject::connect(actionSave_Script, SIGNAL(triggered()), MainWindow, SLOT(saveScript()));
        QObject::connect(actionClear_Output, SIGNAL(triggered()), MainWindow, SLOT(cleanOutput()));
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(actionAbout()));
        QObject::connect(actionRun, SIGNAL(triggered()), MainWindow, SLOT(runScript()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Project Cell Script Tester", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Esc", 0));
        actionLoad_Script->setText(QApplication::translate("MainWindow", "Load Script", 0));
        actionLoad_Script->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave_Script->setText(QApplication::translate("MainWindow", "Save Script", 0));
        actionSave_Script->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionClear_Output->setText(QApplication::translate("MainWindow", "Clean Output", 0));
        actionClear_Output->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionFullscreen->setText(QApplication::translate("MainWindow", "Fullscreen", 0));
        actionFullscreen->setShortcut(QApplication::translate("MainWindow", "F11", 0));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0));
        actionRun->setShortcut(QApplication::translate("MainWindow", "Ctrl+Space", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
