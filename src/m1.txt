
#include <iostream>
#include "lexer/lexer.h"
#include "TokenUtils.h"

int main() {

    std::string source = R"(
        adad age = 20;

        agar (age >= 18) {
            wapas age;
        }
    )";

    Lexer lexer(source);

    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {

        std::cout
            << "Lexeme: " << token.lexeme
            // << " | Type: " << static_cast<int>(token.type)
            << " | Type: " << tokenTypeToString(token.type)
            << " | Line: " << token.line
            << " | Column: " << token.column
            << '\n';
    }

    return 0;
}
