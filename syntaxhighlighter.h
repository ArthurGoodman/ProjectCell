#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text);
};

#endif // SYNTAXHIGHLIGHTER_H
