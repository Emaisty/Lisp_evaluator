#ifndef LISP_PARSER_AST_H
#define LISP_PARSER_AST_H

#include <vector>
#include <iostream>
#include <map>


namespace AST {
    class List {
    public:

        bool quoted = false;

        virtual ~List() {};

        virtual List *clone() = 0;

        virtual List *eval() = 0;

        virtual void print() = 0;

    };

    class Number : public List {
    public:
        Number *clone() override;

        List *eval() override;

        void print() override;

        int value;
    };

    class BinOpr : public List {
    public:
        BinOpr *clone() override;

        List *eval() override;

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
    };

    class Car : public List {
        Car *clone() override;

        List *eval() override;

        void print() override;
    };

    class Cdr : public List {
        Cdr *clone() override;

        List *eval() override;

        void print() override;
    };

    class ObjList : public List {
    public:
        ObjList *clone() override;

        List *eval() override;

        void print() override;

        std::vector<List *> elemList;

    };

    class Var : public List {
    public:
        Var *clone() override;

        List *eval() override;

        void print() override;

        std::string name;
    };


    class Func : public List {
    public:
        Func *clone() override;

        List *eval() override;

        void print() override;

        std::string name;

        std::vector<std::string> args;
        List *body;
    };

}

extern std::map<std::string, AST::List *> globalDefines;

extern std::map<std::string, AST::List *> localVars;


#endif //LISP_PARSER_AST_H
