#include "AST.h"


AST::Number *AST::Number::clone() {
    return new AST::Number(*this);
}

AST::Number *AST::Number::eval() {
    return this->clone();
}

void AST::Number::print() {
    std::cout << value;
}

AST::BinOpr *AST::BinOpr::clone() {
    return new AST::BinOpr(*this);
}

AST::Number *AST::BinOpr::eval() {
    Number *l, *r, res;
    l = dynamic_cast<Number *>(left->eval());
    r = dynamic_cast<Number *>(right->eval());
    if (!l || !r)
        throw "BinOpr Error. Expected number";
    switch (op) {
        case '+':
            res.value = l->value + r->value;
            break;
        case '-':
            res.value = l->value - r->value;
            break;
        case '*':
            res.value = l->value * r->value;
            break;
        case '/':
            res.value = l->value / r->value;
            break;
        case '&':
            res.value = l->value && r->value;
            break;
        case '|':
            res.value = l->value || r->value;
            break;
        case '=':
            res.value = l->value == r->value;
            break;
        case '!':
            res.value = l->value != r->value;
            break;
        case '>':
            switch (add) {
                case '=':
                    res.value = l->value >= r->value;
                    break;
                default:
                    res.value = l->value > r->value;
                    break;
            }
            break;
        case '<':
            switch (add) {
                case '=':
                    res.value = l->value <= r->value;
                    break;
                default:
                    res.value = l->value < r->value;
                    break;
            }
            break;
    }
    return res.clone();
}

void AST::BinOpr::print() {
    std::cout << op << add;
    left->print();
    right->print();
}


AST::IfCond *AST::IfCond::clone() {
    return new AST::IfCond(*this);
}

AST::List *AST::IfCond::eval() {
    Number *condition;
    condition = dynamic_cast<Number *>(cond->eval());
    if (!condition)
        throw "expected Number in condition";

    if (condition->value != 0) {
        return tr->eval();
    } else {
        return fl->eval();
    }
}

void AST::IfCond::print() {
    std::cout << "(if ";
    cond->print();
    std::cout << " then ";
    tr->print();
    std::cout << " else ";
    fl->print();
}