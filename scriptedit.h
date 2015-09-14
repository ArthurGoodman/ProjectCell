#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include <QFrame>
#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>

class ScriptEdit : public QFrame
{
    Q_OBJECT

    class NumberBar : public QWidget
    {
        QTextEdit *edit;
        int highestLine;

    public:
        NumberBar(QWidget *parent = 0);

        void setTextEdit(QTextEdit *edit);
        void update();

    protected:
        void paintEvent(QPaintEvent *);
    };

    class TextEdit : public QTextEdit
    {
        QTextCursor c;

    public:
        TextEdit(QWidget *parent = 0);

    private:
        void updateCursor();
        int pos();
        void insert(QString text);
        void moveCursor(int delta);
        QChar charAt(int pos);
        QChar curChar();
        QChar prevChar();

    protected:
        void keyPressEvent(QKeyEvent *e);
    };

    TextEdit *edit;
    NumberBar *numberBar;
    QHBoxLayout *hBox;

public:
    ScriptEdit(QWidget *parent = 0);
    ~ScriptEdit();

    QTextEdit *textEdit();

protected:
    bool eventFilter(QObject *, QEvent *);
};

#endif // SCRIPTEDIT_H
