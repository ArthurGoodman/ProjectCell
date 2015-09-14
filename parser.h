#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>

#include "nodes.h"

class Expression;

class Parser
{
    enum { tNone = -1,

           tPlus, tMinus, tMultiply, tDivide, tMod, tPower,
           tLt, tGt, tLe, tGe, tEq, tNe,
           tAnd, tOr, tXor, tNot,
           tDec, tInc,
           tAssign, tPeriod, tComma, tSharp, tSemi,
           tLPar, tLBracket, tLBrace, tRPar, tRBracket, tRBrace,

           tIf, tElse, tWhile, tFor, tForever,
           tBreak, tContinue, tReturn, tVar, tFunction,

           tId, tNumber,
           tUnknown, tEnd };

    class Token
    {
        int i; QString t;

     public:
         Token(int id = tNone, QString text = "");

         QString text();

         Token &operator =(int id);
         Token &operator +=(QChar c);

         bool operator ==(int id);
         bool operator <=(int id);
         bool operator >=(int id);
    };

    QStringList operators, keywords;

    QString code;
    Token token;
    int pos;

public:
    Parser();

    Expression *compile(QString code);

    QStringList getKeywords();

private:
    void error(QString msg);
    void getToken();

    bool check(char id);
    bool accept(char id);
    bool range(char a, char b);

    Node *expression();
    Node *logicOr();
    Node *logicXor();
    Node *logicAnd();
    Node *equality();
    Node *relation();
    Node *addSub();
    Node *mulDiv();
    Node *power();
    Node *preffix();
    Node *suffix();
    Node *term();
};

#endif // PARSER_H
