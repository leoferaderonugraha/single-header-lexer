#include "lexer.h"
#include <string>
#include <iostream>
#include <memory>

int
main(int argc, char **argv) {
    std::string data = "Hello, World!\n123\ntest123\n123test";
    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(Lexer(data));

    for (Token &token : lexer->analyze()) {
        std::cout << token.type_literal << " : " << '\'' << token.literal << '\'' << std::endl;
    }
}
