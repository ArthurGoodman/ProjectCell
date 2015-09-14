#include "syntaxhighlighter.h"
#include <QtWidgets>

#include "kernel.h"
#include "parser.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    QStringList keywords = Kernel::parser->getKeywords();

    int n, l = text.length();
    for(int i = 0; i < l; i++)
    {
        n = 0;

        if(text.at(i).isLetter())
        {
            while(i < l && text.at(i).isLetter()) { n++; i++; }
            if(keywords.contains(text.mid(i-n,n))) setFormat(i-n, n, QColor(249, 38, 114));
            //else if(Parser::hasFunction(text.mid(i-n,n))) setFormat(i-n, n, QColor(102, 217, 239));
        }
        else if(text.at(i).isDigit())
        {
            while(i < l && text.at(i).isDigit()) { n++; i++; }
            if(i+1 < l && text.at(i) == '.' && text.at(i+1).isDigit())
            {
                n++; i++;
                while(i < l && text.at(i).isDigit()) { n++; i++; }
            }
            if(i+1 < l && text.at(i).toLower() == 'e' && (text.at(i+1).isDigit() || (i+2 < l && (text.at(i+1) == '+' || text.at(i+1) == '-') && text.at(i+2).isDigit())))
            {
                n++; i++; if(text.at(i) == '+' || text.at(i) == '-') { n++; i++; }
                while(i < l && text.at(i).isDigit()) { n++; i++; }
            }

            setFormat(i-n, n, QColor(127, 129, 255));
        }
        else if(text.at(i) == '#' || (text.at(i) == '/' && text.at(i+1) == '/'))
        {
            if(text.at(i) == '#') { n++; i++; } else { n+=2; i+=2; }
            while(i < l && text.at(i) != 0) { n++; i++; }

            setFormat(i-n, n, QColor(128, 128, 128));
        }
    }
}
