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

    bool Parse();                    // parse
    const llvm::Module &Generate();  // generate

private:

    void match(Token tok);

    void next();

    void parse_func();

    void parse_main();

    AST::List *parse_binop(char c);

    AST::List *parse_command();

    std::vector<AST::Functions> func;
    std::vector<AST::List *> main;


    llvm::LLVMContext MilaContext;   // llvm context
    llvm::IRBuilder<> MilaBuilder;   // llvm builder
    llvm::Module MilaModule;         // llvm module

    Lexer m_Lexer;                   // lexer is used to read tokens
    Token cur_tok;                      // to keep the current token

};

#endif //LISP_PARSER_PARSER_H
