#include "Parser.h"
#include <emmintrin.h>
#include <immintrin.h>

// Use tutorials in: https://llvm.org/docs/tutorial/

int main(int argc, char *argv[]) {
    Parser parser;

    parser.InitLexan(argv[1]);

    if (!parser.Parse()) {
        return 1;
    }

    parser.generate();
    return 0;
}

