
#include <iostream>
#include "lexer.h"
#include "parser.h"

int main() {

    std::string source = R"(
        adad age = 20;
        adad marks = 85;

        agar (age >= 18) {
            marks = marks + 10;
        } warna {
            marks = marks - 5;
        }

        jabtak (age < 25) {
            age = age + 1;
        }

        wapas age;
    )";

    // Phase 1: Lexical Analysis
    Lexer lexer(source);

    std::vector<Token> tokens = lexer.tokenize();

    // Phase 2: Syntax Analysis
    Parser parser(tokens);

    std::vector<StmtPtr> ast = parser.parse();

    std::cout << "\nParsing completed.\n";
    std::cout << "AST statements: "
              << ast.size() << '\n';

    return 0;
}
