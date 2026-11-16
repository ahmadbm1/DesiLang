
#pragma once

#include <string>
#include <vector>
#include "Token.h"

class Lexer {

private:
    std::string source;

    size_t position = 0;

    int line = 1;
    int column = 1;

    char currentChar();

    char peek(size_t offset = 1) const;

    char advance();

    bool isAtEnd() const;

    void skipWhitespaceAndComments();

    Token makeToken(TokenType type,
                    const std::string& lexeme,
                    int startLine,
                    int startColumn);

    Token scanIdentifier();

    Token scanNumber();

    Token scanCharacter();

    Token scanOperatorOrDelimiter();

public:
    explicit Lexer(const std::string& sourceCode);

    std::vector<Token> tokenize();
};
