#include "AST.h"


AST::List_val::List_val() {

}

AST::List_val::~List_val() {

}

void AST::List_val::codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule) {
    auto intType = llvm::IntegerType::get(MilaContext, 32);
    auto listType = llvm::StructType::create(MilaContext, "list");
    auto listPtrType = llvm::PointerType::get(listType, 0) ;
    listType->setBody({intType, intType, listPtrType, listPtrType}, false);

    this->ListType = listType;
    this->ListPtrType = listPtrType;
}

llvm::Value *AST::BinOpr::codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder,
                                  llvm::Module &MilaModule) {
    llvm::Value *L = this->left->codegen(MilaContext, MilaBuilder, MilaModule);
    llvm::Value *R = this->right->codegen(MilaContext, MilaBuilder, MilaModule);
    switch (op) {
        case '+':
            return MilaBuilder.CreateAdd(L, R, "addtmp");
        case '-':
            return MilaBuilder.CreateSub(L, R, "subtmp");
        case '*':
            return MilaBuilder.CreateMul(L, R, "multmp");
        case '/':
            return MilaBuilder.CreateUDiv(L, R, "divtmp");
        case '&':
            L = MilaBuilder.CreateMul(L, R, "multmp");
            L = MilaBuilder.CreateICmpNE(L, llvm::ConstantInt::get(llvm::Type::getInt32Ty(MilaContext),
                                                                   llvm::APInt(32, 0)),
                                         "noteq");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);

        case '|':
            L = MilaBuilder.CreateAdd(L, R, "multmp");
            L = MilaBuilder.CreateICmpNE(L, llvm::ConstantInt::get(llvm::Type::getInt32Ty(MilaContext),
                                                                   llvm::APInt(32, 0)),
                                         "noteq");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);

        case '>':
            L = MilaBuilder.CreateICmpSGT(L, R, "gttmp");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);
        case '<':
            L = MilaBuilder.CreateICmpSLT(L, R, "lttmp");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);
        case '=':
            L = MilaBuilder.CreateICmpEQ(L, R, "eqtmp");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);
            // '!'
        default:
            L = MilaBuilder.CreateICmpNE(L, R, "netmp");
            return MilaBuilder.CreateIntCast(L, llvm::Type::getInt32Ty(MilaContext), false);
    }
}


llvm::Value *AST::IntNumb::codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder,
                                   llvm::Module &MilaModule) {
    return llvm::ConstantInt::get(MilaContext, llvm::APInt(32, value));
}


void AST::Program::setBody(std::vector<AST::Main> main) {
    body = main;
}

void AST::Program::setFunc(std::vector<AST::Functions> f) {
    func = f;
}

void AST::Program::codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule) {

}

//======================================================================================================================

AST::BinOpr *AST::BinOpr::clone() const {
    return new AST::BinOpr(*this);
}

AST::IntNumb *AST::IntNumb::clone() const {
    return new AST::IntNumb(*this);
}