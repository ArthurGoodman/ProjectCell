QT += core gui widgets

TARGET = script-tester
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    context.cpp \
    exceptions.cpp \
    expression.cpp \
    functions.cpp \
    nodes.cpp \
    parser.cpp \
    scriptedit.cpp \
    syntaxhighlighter.cpp \
    kernel.cpp \
    ui.cpp

HEADERS += mainwindow.h \
    context.h \
    exceptions.h \
    expression.h \
    functions.h \
    nodes.h \
    parser.h \
    scriptedit.h \
    syntaxhighlighter.h \
    kernel.h \
    ui.h

FORMS += mainwindow.ui
