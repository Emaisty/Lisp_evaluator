#include "Parser.h"

Parser::Parser() {
}


void Parser::match(Token tok) {
    if (cur_tok != tok) {
        std::cout << "ERROR. Expected: " << tok << " but get " << cur_tok << std::endl;
    }
}

void Parser::next() {
    cur_tok = m_Lexer.gettok();
}


void Parser::InitLexan(char *name_of_file) {
    m_Lexer.InitInput(name_of_file);
}

AST::Func *Parser::function() {

}

AST::IfCond *Parser::if_statement() {
    next();
    AST::IfCond stat;
    stat.cond = parse_list();
    stat.tr = parse_list();
    stat.fl = parse_list();
    return stat.clone();
}

AST::List *Parser::binopr() {
    AST::BinOpr res;
    switch (cur_tok) {
        case tok_plus:
            res.op = '+';
            break;
        case tok_minus:
            res.op = '-';
            break;
        case tok_mul:
            res.op = '*';
            break;
        case tok_div:
            res.op = '/';
            break;
        case tok_equal:
            res.op = '=';
            break;
        case tok_notequal:
            res.op = '!';
            break;
        case tok_and:
            res.op = '&';
            break;
        case tok_or:
            res.op = '|';
            break;
        case tok_greaterequal:
            res.add = '=';
        case tok_greater:
            res.op = '>';
            break;
        case tok_lessequal:
            res.add = '=';
        case tok_less:
            res.op = '<';
            break;
        default:
            throw "unknown bin opr";
    }
    next();
    res.left = parse_list();
    res.right = parse_list();
    return res.clone();
}

AST::Number *Parser::number() {
    AST::Number res;
    switch (cur_tok) {
        case tok_false:
            res.value = 0;
            break;
        case tok_true:
            res.value = 1;
            break;
        case tok_number_int:
            res.value = m_Lexer.numVal();
            break;
        default:
            throw "Expect number";
    }
    next();
    return res.clone();
}

AST::List *Parser::quot_list() {

}

AST::List *Parser::command_list() {
    switch (cur_tok) {
        case tok_if:
            return if_statement();
            break;
        case tok_function:
            break;
        case tok_plus:
        case tok_minus:
        case tok_mul:
        case tok_div:
        case tok_equal:
        case tok_notequal:
        case tok_greaterequal:
        case tok_greater:
        case tok_lessequal:
        case tok_less:
            return binopr();
        default:
            throw "Unknown command";
    }
}

AST::List *Parser::parse_list() {
    AST::List *res = nullptr;
    switch (cur_tok) {
        case tok_quot:
            next();
            match(tok_opbrak);
            next();
            res = quot_list();
            match(tok_clbrak);
            next();
            break;
        case tok_number_int:
        case tok_true:
        case tok_false:
            res = number();
            break;
        default:
            match(tok_opbrak);
            next();
            res = command_list();
            match(tok_clbrak);
            next();
            break;
    }
    return res;
}

bool Parser::Parse() {
    next();
    while (cur_tok != tok_eof) {
        lists.push_back(parse_list());
    }
    return true;
}

void Parser::generate() {
    for (auto &list: lists) {
        list->eval()->print();
        std::cout << std::endl;
    }
}