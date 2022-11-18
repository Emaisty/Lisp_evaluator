#include "Parser.h"

// Use tutorials in: https://llvm.org/docs/tutorial/

int main(int argc, char *argv[]) {
//TODO ASK HERE!!!!!!!!!!!!!
//    if (argc == 1)
//        return 0;
    Parser parser;

    parser.InitLexan(argv[1]);

    if (!parser.Parse()) {
        return 1;
    }

    parser.Generate().print(llvm::outs(), nullptr);

    return 0;
}

