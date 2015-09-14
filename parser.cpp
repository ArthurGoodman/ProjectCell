#include "parser.h"
#include <QtWidgets>

#include "exceptions.h"
#include "expression.h"
#include "functions.h"
#include "ui.h"
#include "consoleparser.h"

Parser::Token::Token(int id, QString text) :
    i(id), t(text)
{
}

QString Parser::Token::text()
{
    return t;
}

Parser::Token &Parser::Token::operator =(int id)
{
    i = id;
    return *this;
}

Parser::Token &Parser::Token::operator +=(QChar c)
{
    t += c;
    return *this;
}

bool Parser::Token::operator ==(int id)
{
    return i == id;
}

bool Parser::Token::operator <=(int id)
{
    return i <= id;
}

bool Parser::Token::operator >=(int id)
{
    return i >= id;
}

Parser::Parser()
{
    operators << "+" << "-" << "*" << "/" << "%" << "^"
              << "<" << ">" << "<=" << ">=" << "==" << "!="
              << "&" << "|" << "||" << "!"
              << "--" << "++"
              << "=" << "." << "," << "#" << ";"
              << "(" << "[" << "{" << ")" << "]" << "}";

    keywords << "if" << "else" << "while" << "for" << "forever"
             << "break" << "continue" << "return" << "var" << "function";
}

Expression *Parser::compile(QString code)
{
    this->code = code;
    pos = 0;

    Expression *expr = new Expression;
    BlockNode *node = (BlockNode *)(expr->root = new BlockNode);

    getToken();

    try
    {
        while(!check(tEnd))
        {
            node->addNode(expression());
            accept(tSemi);
        }
    }
    catch(ErrorException *e)
    {
        expr->msg = e->message();
        delete e;
    }
    catch(...)
    {
        expr->msg = "Whoops...";
    }

    return expr;
}

QStringList Parser::getKeywords()
{
    return keywords;
}

void Parser::error(QString msg)
{
    throw new SyntaxError(msg);
}

void Parser::getToken()
{
    token = Token();

    while(code[pos].isSpace()) ++pos;
    while(code[pos] == '#' || (code[pos] == '/' && code[pos+1] == '/'))
    {
                if(code[pos] == '#')
        {
            QString cmd; while(!code[++pos].isNull() && code[pos] != '\n') cmd += code[pos];
            Ui::consoleParser->execute(cmd);
        }
        else while(!code[pos].isNull() && code[pos] != '\n') ++pos;

        while(code[pos].isSpace()) ++pos;
    }

    if(code[pos].isNull()) token = tEnd;
    else if(code[pos].isDigit())
    {
        token = tNumber; do token += code[pos++]; while(code[pos].isDigit());
        if(code[pos] == '.' && code[pos+1].isDigit()) do token += code[pos++]; while(code[pos].isDigit());
        if(code[pos].toLower() == 'e' && (((code[pos+1] == '+' || code[pos+1] == '-') && code[pos+2].isNumber()) || code[pos+1].isNumber()))
        {
            token += code[pos++]; if(code[pos] == '+' || code[pos] == '-') token += code[pos++];
            while(code[pos].isDigit()) token += code[pos++];
        }
    }
    else if(code[pos].isLetter() || code[pos] == '_')
    {
        do token += code[pos++]; while(code[pos].isLetterOrNumber() || code[pos] == '_');
        int i; if((i = keywords.indexOf(token.text())) + 1) token = tIf + i; else token = tId;
    }
    else
    {
        token += code[pos++];

        if(QString("<>=!").contains(token.text()) && code[pos] == '=') token += code[pos++];
        else if((token.text() == "|" && code[pos] == '|') || ((token.text() == "+" || token.text() == "-") && token.text()[0] == code[pos]))
            token += code[pos++];

        int i; if((i = operators.indexOf(token.text())) + 1) token = tPlus + i; else token = tId;
    }
}

bool Parser::check(char id)
{
    return token == id;
}

bool Parser::accept(char id)
{
    if(token == id)
    {
        getToken();
        return true;
    }

    return false;
}

bool Parser::range(char a, char b)
{
    return token >= a && token <= b;
}

Node *Parser::expression()
{
    BlockNode *node = new BlockNode;
    do node->addNode(logicOr()); while(accept(tComma));
    return node;
}

Node *Parser::logicOr()
{
    Node *node = logicXor();
    while(accept(tOr)) node = new OrNode(node, logicXor());
    return node;
}

Node *Parser::logicXor()
{
    Node *node = logicAnd();
    while(accept(tXor)) node = new XorNode(node, logicAnd());
    return node;
}

Node *Parser::logicAnd()
{
    Node *node = equality();
    while(accept(tAnd)) node = new AndNode(node, equality());
    return node;
}

Node *Parser::equality()
{
    Node *node = relation();

    while(range(tEq, tNe))
    {
        if(accept(tEq)) node = new EqNode(node, relation());
        else if(accept(tNe)) node = new NeNode(node, relation());
    }

    return node;
}

Node *Parser::relation()
{
    Node *node = addSub();

    while(range(tLt, tGe))
    {
        if(accept(tLt)) node = new LtNode(node, addSub());
        else if(accept(tGt)) node = new GtNode(node, addSub());
        else if(accept(tLe)) node = new LeNode(node, addSub());
        else if(accept(tGe)) node = new GeNode(node, addSub());
    }

    return node;
}

Node *Parser::addSub()
{
    Node *node = mulDiv();

    while(range(tPlus, tMinus))
    {
        if(accept(tPlus)) node = new PlusNode(node, mulDiv());
        else if(accept(tMinus)) node = new MinusNode(node, mulDiv());
    }

    return node;
}

Node *Parser::mulDiv()
{
    Node *node = power();

    while(range(tMultiply, tMod))
    {
        if(accept(tMultiply)) node = new MultiplyNode(node, power());
        else if(accept(tDivide)) node = new DivideNode(node, power());
        else if(accept(tMod)) node = new ModNode(node, power());
    }

    return node;
}

Node *Parser::power()
{
    Node *node = preffix();
    while(accept(tPower)) node = new PowerNode(node, preffix());
    return node;
}

Node *Parser::preffix()
{
    Node *node = 0;

    if(accept(tPlus)) node = suffix();
    else if(accept(tMinus)) node = new UnaryMinusNode(suffix());
    else if(accept(tNot)) node = new NotNode(suffix());
    else if(accept(tDec))
    {
        node = dynamic_cast<VariableNode *>(suffix());
        if(!node) error("variable expected after preffix decrement");
        node = new PrefDecNode(((VariableNode *)node)->getName());
    }
    else if(accept(tInc))
    {
        node = dynamic_cast<VariableNode *>(suffix());
        if(!node) error("variable expected after preffix increment");
        node = new PrefIncNode(((VariableNode *)node)->getName());
    }
    else node = suffix();

    return node;
}

Node *Parser::suffix()
{
    Node *node = term();

    if(accept(tDec))
    {
        node = dynamic_cast<VariableNode *>(node);
        if(!node) error("variable expected before suffix decrement");
        node = new SuffDecNode(((VariableNode *)node)->getName());
    }
    else if(accept(tInc))
    {
        node = dynamic_cast<VariableNode *>(node);
        if(!node) error("variable expected before suffix increment");
        node = new SuffIncNode(((VariableNode *)node)->getName());
    }

    return node;
}

Node *Parser::term()
{
    Node *node = 0;

    if(check(tNumber)) { node = new ValueNode(token.text().toFloat()); getToken(); }

    else if(check(tId))
    {
        QString name = token.text();
        getToken();

        if(accept(tLPar))
        {
            QList<Node *> args;

            if(!accept(tRPar))
            {
                args = ((BlockNode *)expression())->getNodes();
                if(!accept(tRPar)) error("unmathced parentheses");
            }

            node = new CallNode(name, args);
        }
        else
        {
            if(accept(tAssign)) node = logicOr();
            node = new VariableNode(name, node);
        }
    }

    else if(accept(tVar))
    {
        QString name; Node *expr = 0;

        node = new BlockNode;

        do
        {
            if(check(tId)) name = token.text();
            else error("identifier expected");

            getToken();

            if(accept(tAssign)) expr = logicOr();

            ((BlockNode *)node)->addNode(new VariableDefinitionNode(name, expr));
        }
        while(accept(tComma));
    }

    else if(accept(tFunction))
    {
        QString name;
        QStringList params;

        if(!check(tId)) error("function name expected");

        name = token.text();
        getToken();

        if(!accept(tLPar)) error("left parenthesis expected");

        while(!accept(tRPar)) do { params << token.text(); getToken(); }
        while(accept(tComma));

        node = new FunctionDefinitionNode(name, params, logicOr());
    }

    else if(accept(tLPar)) { node = expression(); if(!accept(tRPar)) error("unmatched parentheses"); }
    else if(accept(tLBrace))
    {
        QList<Node *> nodes; while(!check(tRBrace) && !check(tEnd)) { nodes << expression(); accept(tSemi); }
        if(!accept(tRBrace)) error("unmatched braces"); node = new BlockNode(nodes, true);
    }

    else if(accept(tIf)) node = new IfNode(logicOr(), logicOr(), accept(tElse) ? logicOr() : 0);

    else if(accept(tWhile)) node = new WhileNode(logicOr(), logicOr());
    else if(accept(tFor))
    {
        Node *init, *cond, *inc;

        if(!accept(tLPar)) error("left parenthesis expected");
        init = logicOr();
        if(!accept(tSemi)) error("semicolon expected");
        cond = logicOr();
        if(!accept(tSemi)) error("semicolon expected");
        inc = logicOr();
        if(!accept(tRPar)) error("unmatched parentheses");

        node = new ForNode(init, cond, inc, logicOr());
    }

    else if(accept(tForever)) node = new WhileNode(new ValueNode(1), logicOr());

    else if(accept(tBreak)) node = new BreakNode(check(tRPar) || check(tRBrace) || check(tEnd) || check(tSemi) ? 0 : logicOr());
    else if(accept(tReturn)) node = new ReturnNode(check(tRPar) || check(tRBrace) || check(tEnd) || check(tSemi) ? 0 : logicOr());
    else if(accept(tContinue)) node = new ContinueNode;

    else if(check(tEnd)) error("unexpected end of expression");
    else if(check(tUnknown)) error("unknown token '" + token.text() + "'");
    else error("unexpected token '" + token.text() + "'");

    return node;
}
