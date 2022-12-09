#ifndef LISP_PARSER_PARSER_H
#define LISP_PARSER_PARSER_H

#include <vector>

#include "AST.h"
#include "Lexer.h"

class Parser {
public:

    Parser();

    ~Parser() = default;

    void InitLexan(char *name_of_file);

    bool Parse();

    void generate();

private:

    void quot_unquot();

    void match(Token tok);

    void next();

    AST::Func *function();

    AST::IfCond *if_statement();

    AST::Number *number();

    AST::Var* var();

    AST::List *binopr();

    AST::List *command();

    AST::List *list();

    AST::List *parse_list();

    std::vector<AST::List *> lists;

    bool if_quot = false;

    Lexer m_Lexer;                   // lexer is used to read tokens
    Token cur_tok;                      // to keep the current token


};

#endif //LISP_PARSER_PARSER_H
