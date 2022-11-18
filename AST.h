#ifndef LISP_PARSER_AST_H
#define LISP_PARSER_AST_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

namespace AST {

    // create list
    /*
    struct List_val{
        int value;
        int isList;
        List_val* First;
        List_val* Second;
    */
    class List_val {
    public:
        llvm::StructType *ListType = nullptr;
        llvm::PointerType *ListPtrType = nullptr;


        List_val();

        ~List_val();

        void codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule);
    };


    static List_val list;

    class List {
    public:
        virtual ~List() = default;

        virtual List *clone() const = 0;

        virtual llvm::Value *
        codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule) = 0;
    };

    class BinOpr : public List {
    public:
        BinOpr *clone() const override;

        llvm::Value *
        codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule) override;

        char op;
        List *left, *right;

    };

    class IntNumb : public List {
    public:

        IntNumb *clone() const override;

        llvm::Value *
        codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule) override;

        int value;
    };


    //==================================================================================================================
    //body
    class Functions {

    };

    class Main {

    };

    class Program {
    private:
        std::vector<Functions> func;
        std::vector<Main> body;
    public:
        void setFunc(std::vector<Functions> f);

        void setBody(std::vector<Main> main);

        void codegen(llvm::LLVMContext &MilaContext, llvm::IRBuilder<> &MilaBuilder, llvm::Module &MilaModule);

    };
}


#endif //LISP_PARSER_AST_H
