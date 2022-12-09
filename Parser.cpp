#include "Parser.h"

Parser::Parser() {
}


void Parser::quot_unquot() {
//    switch (cur_tok) {
//        case tok_unquot:
//            if (!if_quot)
//                throw std::invalid_argument("try to unquoting not quoted");
//            pr_state = if_quot;
//            if_quot = false;
//            res = parse_list();
//            if_quot = pr_state;
//            break;
//        case tok_quot:
//            next();
//            pr_state = if_quot;
//            if_quot = true;
//            res = parse_list();
//            if_quot = pr_state;
//            break;
//    }
}

void Parser::match(Token tok) {
    if (cur_tok != tok) {
        throw std::invalid_argument("Error. Unexpected token");
    }
}

void Parser::next() {
    cur_tok = m_Lexer.gettok();
}


void Parser::InitLexan(char *name_of_file) {
    m_Lexer.InitInput(name_of_file);
}

AST::Func *Parser::function() {
    AST::Func res;
    next();
    match(tok_opbrak);
    next();
    match(tok_identifier);
    res.name = m_Lexer.identifierStr();
    next();
    while (cur_tok != tok_clbrak) {
        match(tok_identifier);
        res.args.push_back(m_Lexer.identifierStr());
        next();
    }
    match(tok_clbrak);
    next();
    res.body = parse_list();
    return res.clone();
}

AST::IfCond *Parser::if_statement() {
    next();
    AST::IfCond res;
//    res.cond = parse_list();
//    res.tr = parse_list();
//    res.fl = parse_list();
//    res.quoted = if_quot;
    return res.clone();
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
    res.quoted = if_quot;
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
    res.quoted = if_quot;
    return res.clone();
}

AST::Var *Parser::var() {
    AST::Var res;
    res.name = m_Lexer.identifierStr();
    res.quoted = if_quot;
    next();
    return res.clone();
}

AST::List *Parser::list() {
    match(tok_opbrak);
    next();
    AST::ObjList res;
    while (cur_tok != tok_clbrak) {
        res.elemList.push_back(parse_list());
    }
    res.quoted = if_quot;
    match(tok_clbrak);
    next();
    return res.clone();
}

AST::List *Parser::command() {
    bool pr_state;
    switch (cur_tok) {
        case tok_if:
            return if_statement();
        case tok_function:
            return function();
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
            return list();
    }
}


AST::List *Parser::parse_list() {
    AST::List *res = nullptr;
    bool pr_quot;
    switch (cur_tok) {
        case tok_quot:
            pr_quot = if_quot;
            if_quot = true;
            next();
            res = parse_list();
            if_quot = pr_quot;
            break;
        case tok_unquot:
            if (!if_quot)
                throw std::invalid_argument("cannot unqout not quoted");
            pr_quot = if_quot;
            if_quot = false;
            next();
            res = parse_list();
            if_quot = pr_quot;
            break;
        case tok_identifier:
            res = var();
        case tok_string:
            break;
        case tok_number_int:
        case tok_true:
        case tok_false:
            res = number();
            break;
        default:
            res = command();
            break;
    }
    return res;
}

bool Parser::Parse() {
    next();
    while (cur_tok != tok_eof) {
        if_quot = false;
        lists.push_back(parse_list());
    }
    return true;
}

void Parser::generate() {
    {
        AST::Func if_cond;
        AST::IfCond body;
        if_cond.body = &body;
        if_cond.args = std::vector<std::string>{"a", "b", "c"};
        globalDefines["if"] = if_cond.clone();
    }
    {
        AST::Func car;
        AST::Car body;
        car.body = &body;
        car.args = std::vector<std::string>{"a"};
        globalDefines["car"] = car.clone();
    }
    {
        AST::Func cdr;
        AST::Cdr body;
        cdr.body = &body;
        cdr.args = std::vector<std::string>{"a"};
        globalDefines["cdr"] = cdr.clone();
    }

    for (auto &list: lists) {
        list->eval()->print();
        std::cout << std::endl;
    }
}