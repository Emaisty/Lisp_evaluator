#ifndef LISP_PARSER_AST_H
#define LISP_PARSER_AST_H

#include <vector>
#include <iostream>

namespace AST {
    class List {
    public:
        List *car;
        List *cdr;

        virtual ~List() {};

        virtual List *clone() = 0;

        virtual List *eval() = 0;

        virtual void print() = 0;
    };

    class Number : public List {
    public:
        Number *clone() override;

        Number *eval() override;

        void print() override;

        int value;
    };

    class BinOpr : public List {
    public:
        BinOpr *clone() override;

        Number *eval() override;

        void print() override;

        List *left;
        List *right;
        char op, add = ' ';
    };

    class IfCond : public List {
    public:
        IfCond *clone() override;

        List *eval() override;

        void print() override;

        List *cond;
        List *tr, *fl;
    };

    class Var : public List {
    public:
        Var *clone() override {};

        Var *eval() override {};

        void print() override {};

        std::string name;
    };


    class Func : public List {
    public:
        Func *clone() override {};

        List *eval() override {};

        void print() override {};

        std::vector<Var *> args;
        List *body;
    };


}


#endif //LISP_PARSER_AST_H
