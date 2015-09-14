#include "scriptedit.h"
#include <QtWidgets>

ScriptEdit::NumberBar::NumberBar(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QWidget { background: rgba(30, 30, 30, 200) }");
    edit = 0; highestLine = 0;
}

void ScriptEdit::NumberBar::setTextEdit(QTextEdit *edit)
{
    this->edit = edit;
}

void ScriptEdit::NumberBar::update()
{
    int w = fontMetrics().width(QString::number(highestLine)) + 22;
    if(width() != w) setFixedWidth(w); QWidget::update();
}

void ScriptEdit::NumberBar::paintEvent(QPaintEvent *event)
{
    int contentsY = edit->verticalScrollBar()->value();
    int pageBottom = contentsY + edit->viewport()->height();

    QFontMetrics fm = fontMetrics();
    QTextBlock currentBlock = edit->document()->findBlock(edit->textCursor().position());

    QPainter painter(this);
    painter.setPen(QColor(230, 230, 230));

    int lineCount = 0;
    QTextBlock block = edit->document()->begin();

    while(block.isValid())
    {
        lineCount++;
        QPointF position = edit->document()->documentLayout()->blockBoundingRect(block).topLeft();

        if(position.y() > pageBottom) break;

        bool bold = false;
        if(block == currentBlock) { bold = true; QFont font = painter.font(); font.setBold(true); painter.setFont(font); }

        if(block == currentBlock) painter.fillRect(0, position.y() - contentsY, width(), fm.height(), QColor(70, 70, 70));
        painter.drawText(width() - fm.width(QString::number(lineCount)) - 14, position.y() - contentsY + fm.ascent(), QString::number(lineCount));

        if(bold) { QFont font = painter.font(); font.setBold(false); painter.setFont(font); }

        block = block.next();
    }

    highestLine = lineCount;
    painter.end();

    QWidget::paintEvent(event);
}

ScriptEdit::TextEdit::TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    setFont(QFont("Courier New", 10));
    setStyleSheet("QTextEdit { color: rgb(230, 230, 230) }");
}

void ScriptEdit::TextEdit::updateCursor()
{
    c = textCursor();
}

int ScriptEdit::TextEdit::pos()
{
    return c.position();
}

void ScriptEdit::TextEdit::insert(QString text)
{
    c.insertText(text);
}

void ScriptEdit::TextEdit::moveCursor(int delta)
{
    if(delta > 0) c.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, abs(delta));
    else if(delta < 0) c.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, abs(delta));
    setTextCursor(c);
}

QChar ScriptEdit::TextEdit::charAt(int pos)
{
    if(pos >= 0 && pos < c.block().length() - 1) return c.block().text().at(pos);
    else return QChar();
}

QChar ScriptEdit::TextEdit::curChar()
{
    return charAt(c.positionInBlock());
}

QChar ScriptEdit::TextEdit::prevChar()
{
    return charAt(c.positionInBlock() - 1);
}

void ScriptEdit::TextEdit::keyPressEvent(QKeyEvent *e)
{
    updateCursor();

    switch(e->key())
    {
    case Qt::Key_Space:
        if(!(e->modifiers() & Qt::ControlModifier)) QTextEdit::keyPressEvent(e);
        else QWidget::keyPressEvent(e); break;

    case Qt::Key_Tab: insert("    "); break;

    case Qt::Key_ParenLeft:
        if(!curChar().isLetterOrNumber()) { insert("()"); moveCursor(-1); }
        else QTextEdit::keyPressEvent(e); break;

    case Qt::Key_ParenRight:
        if(curChar() == ')') moveCursor(1);
        else QTextEdit::keyPressEvent(e); break;

    case Qt::Key_BraceLeft:
        insert("{}"); moveCursor(-1); break;

    case Qt::Key_BraceRight:
        if(curChar() == '}') moveCursor(1);
        else QTextEdit::keyPressEvent(e); break;

    case Qt::Key_Return:
    {
        QString text = document()->toPlainText(), space = "";
        int n = 0;

        for(int i = 0; i < pos(); i++) if(text.at(i) == '{') n++; else if(text.at(i) == '}') n--;
        space.fill(' ', 4*n);

        if(prevChar() == '{')
        {
            insert("\n" + space);
            if(curChar() == '}')
            {
                insert("\n" + space.left(space.length()-4));
                moveCursor(3-4*n);
            }
            break;
        }
        else if(curChar() == '}') { insert("\n" + space.left(space.length()-4)); break; }
        else { insert("\n" + space); break; }

        QTextEdit::keyPressEvent(e); break;
    }

    default: QTextEdit::keyPressEvent(e);
    }
}

ScriptEdit::ScriptEdit(QWidget *parent) :
    QFrame(parent)
{
    edit = new TextEdit;

    edit->setFrameStyle(QFrame::NoFrame);
    edit->setAcceptRichText(false);
    edit->setWordWrapMode(QTextOption::NoWrap);

    numberBar = new NumberBar;
    numberBar->setTextEdit(edit);
    numberBar->setFont(edit->font());

    hBox = new QHBoxLayout(this);
    hBox->setSpacing(0);
    hBox->setMargin(0);
    hBox->addWidget(numberBar);
    hBox->addWidget(edit);

    edit->installEventFilter(this);
    edit->viewport()->installEventFilter(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QFrame { background: rgba(40, 40, 40, 200) }");
}

ScriptEdit::~ScriptEdit()
{
    //    delete numberBar;
    //    delete hbox;
}

QTextEdit *ScriptEdit::textEdit()
{
    return edit;
}

bool ScriptEdit::eventFilter(QObject *object, QEvent *event)
{
    if(object == edit || object == edit->viewport()) { numberBar->update(); return false; }
    return QObject::eventFilter(object, event);
}
