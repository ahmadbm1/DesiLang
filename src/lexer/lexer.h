
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../include/Token.h"

class Lexer {

private:
    std::string source;

    size_t position = 0;

    int line = 1;
    int character = 1;

    bool isAtEnd(size_t offset=0) const;
    char currentChar();
    
    char peek(size_t offset = 1) const;

    char next();


    void skipWhitespaceAndComments();

    Token scanKeywordIdentifier();

    Token scanNumber();

    Token scanCharacter();

    Token scanOperatorOrDelimiter();

    const std::unordered_map<std::string, TokenType> keywords = {
    {"adad", TokenType::KwAdad},
    {"ashariya", TokenType::KwAshariya},
    {"harf", TokenType::KwHarf},
    {"mantiqi", TokenType::KwMantiqi},
    {"mustaqil", TokenType::KwMustaqil},

    {"agar", TokenType::KwAgar},
    {"warna", TokenType::KwWarna},
    {"jabtak", TokenType::KwJabtak},
    {"ke_liye", TokenType::KwKeLiye},
    {"wapas", TokenType::KwWapas},

    {"sach", TokenType::KwSach},
    {"jhoot", TokenType::KwJhoot}
};

public:
    explicit Lexer(const std::string& sourceCode);

    std::vector<Token> tokenize();
};
