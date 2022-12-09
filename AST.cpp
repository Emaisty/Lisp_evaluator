#include "AST.h"

std::map<std::string, AST::List *> globalDefines;

std::map<std::string, AST::List *> localVars;


AST::Number *AST::Number::clone() {
    return new AST::Number(*this);
}

AST::List *AST::Number::eval() {
    return this->clone();
}

void AST::Number::print() {
    std::cout << value << ' ';
}

AST::BinOpr *AST::BinOpr::clone() {
    return new AST::BinOpr(*this);
}

AST::List *AST::BinOpr::eval() {
    if (quoted) {
        AST::ObjList res;
        AST::Var str;
        str.quoted = true;
        str.name = op;
        str.name += add;
        res.elemList.push_back(str.clone());
        res.elemList.push_back(left->clone());
        res.elemList.push_back(right->clone());
        return res.clone();
    }
    Number *l, *r, res;
    l = dynamic_cast<Number *>(left->eval());
    r = dynamic_cast<Number *>(right->eval());
    if (!l || !r)
        throw std::invalid_argument("BinOpr Error. Expected number");
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
    throw std::invalid_argument("Cannot print not quoted bin oper");
}

AST::IfCond *AST::IfCond::clone() {
    return new AST::IfCond(*this);
}

AST::List *AST::IfCond::eval() {
    if (quoted) {
        AST::ObjList res;
        AST::Var str;
        str.quoted = true;
        str.name = "if";
        res.elemList.push_back(str.clone());
        res.elemList.push_back(localVars["a"]->eval());
        res.elemList.push_back(localVars["b"]->eval());
        res.elemList.push_back(localVars["c"]->eval());
        return res.clone();
    }
    Number *condition;
    condition = dynamic_cast<Number *>(localVars["a"]->eval());

    if (!condition || condition->value != 0) {
        return localVars["b"]->eval();
    } else {
        return localVars["c"]->eval();
    }
}

void AST::IfCond::print() {
    throw std::invalid_argument("Cannot print not quoted if statement");
}

AST::Car *AST::Car::clone() {
    return new AST::Car(*this);
}

AST::List *AST::Car::eval() {
    if (quoted) {
        AST::ObjList res;
        AST::Var str;
        str.quoted = true;
        str.name = "car";
        res.elemList.push_back(str.clone());
        res.elemList.push_back(localVars["a"]->eval());
        return res.clone();
    }
    ObjList *lst;
    lst = dynamic_cast<ObjList *>(localVars["a"]->eval());
    if (!lst)
        throw std::invalid_argument("cannot take car from not list object");
    if (lst->elemList.empty())
        throw std::invalid_argument("cannot take car null list");
    return lst->elemList[0];
}

void AST::Car::print() {
    throw std::invalid_argument("Cannot print not quoted car statement");
}

AST::Cdr *AST::Cdr::clone() {
    return new AST::Cdr(*this);
}

AST::List *AST::Cdr::eval() {
    if (quoted) {
        AST::ObjList res;
        AST::Var str;
        str.quoted = true;
        str.name = "cdr";
        res.elemList.push_back(str.clone());
        res.elemList.push_back(localVars["a"]->eval());
        return res.clone();
    }
    ObjList *lst;
    lst = dynamic_cast<ObjList *>(localVars["a"]->eval());
    if (!lst)
        throw std::invalid_argument("cannot take cdr from not list object");
    if (lst->elemList.empty())
        throw std::invalid_argument("cannot take cdr null list");
    if (lst->elemList.size() == 1)
        throw std::invalid_argument("cannot take cdr from 1 elem list");
    if (lst->elemList.size() == 2)
        return lst->elemList[1]->eval();
    ObjList res;
    res.quoted = lst->quoted;
    for (int i = 1; i < lst->elemList.size(); ++i) {
        res.elemList.push_back(lst->elemList[i]->clone());
    }
    return res.clone();
}

void AST::Cdr::print() {
    throw std::invalid_argument("Cannot print not quoted cdr statement");
}


AST::ObjList *AST::ObjList::clone() {
    return new AST::ObjList(*this);
}

AST::List *AST::ObjList::eval() {
    BinOpr *binOperator = dynamic_cast<BinOpr *>(elemList[0]);
    if (binOperator && elemList.size() == 1)
        return binOperator->eval();
    if (quoted)
        return this->clone();
    if (elemList.empty())
        throw std::invalid_argument("missing procedure expression");
    Func *command = dynamic_cast<Func *>(elemList[0]->eval());
    if (!command)
        throw std::invalid_argument("expected a procedure that can be applied to arguments");
    if (command->args.size() != elemList.size() - 1)
        throw std::invalid_argument("the expected number of arguments does not match the given number");
    for (int i = 0; i < command->args.size(); ++i) {
        localVars[command->args[i]] = elemList[i + 1];
    }
    List *res = command->body->eval();
    localVars.clear();
    return res;
}

void AST::ObjList::print() {
    if (elemList.size() == 1) {
        elemList[0]->eval()->print();
        return;
    }
    std::cout << "( ";
    for (auto &i: elemList) {
        i->eval()->print();
    }
    std::cout << ") ";
}


AST::Var *AST::Var::clone() {
    return new AST::Var(*this);
}

AST::List *AST::Var::eval() {
    if (quoted)
        return this->clone();
    if (localVars.find(name) != localVars.end())
        return localVars[name]->eval();
    if (globalDefines.find(name) != globalDefines.end())
        return globalDefines[name]->eval();
    throw std::invalid_argument("Unknown reference " + name);
}

void AST::Var::print() {
    std::cout << name << ' ';
}

AST::Func *AST::Func::clone() {
    return new AST::Func(*this);
}

AST::List *AST::Func::eval() {
    if (quoted) {
        AST::Var res;
        res.name = "#<procedure:" + this->name + ">";
        return res.clone();
    }
    globalDefines[name] = this;
    return this;
}

void AST::Func::print() {

}
