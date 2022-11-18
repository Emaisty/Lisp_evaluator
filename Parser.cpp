#include "Parser.h"

Parser::Parser()
        : MilaContext(), MilaBuilder(MilaContext), MilaModule("racket", MilaContext) {
}


void Parser::match(Token tok) {
    if (cur_tok != tok) {
        std::cout << "ERROR. Expected: " << tok << " but get " << cur_tok << std::endl;
    }
}

void Parser::next() {
    cur_tok = m_Lexer.gettok();
}

AST::List *Parser::parse_binop(char c) {
    AST::BinOpr opr;
    opr.op = c;
    next();
    opr.left = parse_command();
    opr.right = parse_command();
    return opr.clone();
}


AST::List *Parser::parse_command() {
    //TODO Quoting

    if (cur_tok == tok_number_int) {
        AST::IntNumb number;
        number.value = m_Lexer.numVal();
        next();
        return number.clone();
    }

    match(tok_opbrak);
    next();
    AST::List *res = nullptr;
    switch (cur_tok) {
        case tok_function:
            //TODO define
            break;
        case tok_plus:
            res = parse_binop('+');
            break;
        case tok_minus:
            res = parse_binop('-');
            break;
        case tok_mul:
            res = parse_binop('*');
            break;
        case tok_div:
            res = parse_binop('/');
            break;
        case tok_and:
            res = parse_binop('&');
            break;
        case tok_or:
            res = parse_binop('+');
            break;

    }
    match(tok_clbrak);
    next();
    return res;
}


void Parser::InitLexan(char *name_of_file) {
    m_Lexer.InitInput(name_of_file);
}

bool Parser::Parse() {
    next();
    while (cur_tok != tok_eof) {
        main.push_back(parse_command());
    }
    return true;
}

const llvm::Module &Parser::Generate() {

    // create writeln function
    {
        std::vector<llvm::Type *> Ints(1, llvm::Type::getInt32Ty(MilaContext));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), Ints, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "writeln", MilaModule);
        for (auto &Arg: F->args())
            Arg.setName("x");
    }

    {
        std::vector<llvm::Type *> Ints(1, llvm::Type::getInt32PtrTy(MilaContext));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), Ints, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "readln", MilaModule);
        for (auto &Arg: F->args())
            Arg.setName("x");
    }

    // create list

    AST::list.codegen(MilaContext, MilaBuilder, MilaModule);

    llvm::GlobalVariable *g;
    g = new llvm::GlobalVariable(MilaModule, AST::list.ListType,
                                 false,
                                 llvm::GlobalValue::ExternalLinkage,
                                 nullptr,
                                 "a");

    g->setInitializer(llvm::ConstantAggregateZero::get(AST::list.ListType));


    // create main function
    {
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), false);
        llvm::Function *MainFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", MilaModule);

        // block
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(MilaContext, "entry", MainFunction);
        MilaBuilder.SetInsertPoint(BB);


        llvm::Value *i32zero = llvm::ConstantInt::get(MilaContext, llvm::APInt(32, 0));
        llvm::Value *indices[2] = {i32zero, llvm::ConstantInt::get(MilaContext, llvm::APInt(32, 0))};
        llvm::Value *i = MilaBuilder.CreateInBoundsGEP(g, llvm::ArrayRef<llvm::Value *>(indices, 2));

        MilaBuilder.CreateStore(i32zero, i);


        MilaBuilder.CreateCall(MilaModule.getFunction("readln"), {
                i
        });

        llvm::Value *U = MilaBuilder.CreateLoad(i->getType()->getPointerElementType(), i, "k");


        // call writeln with value from lexel
        MilaBuilder.CreateCall(MilaModule.getFunction("writeln"), {
                U
        });
        for (auto &i: main) {
            MilaBuilder.CreateCall(MilaModule.getFunction("writeln"), {
                    i->codegen(MilaContext, MilaBuilder, MilaModule)
            });
        }

        // return 0
        MilaBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(MilaContext), 0));
    }

    return this->MilaModule;
}
