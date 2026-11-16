
#pragma once

#include <string>

enum class TokenType {

    // Keywords
    KwAdad,         // adad (integer)
    KwAshariya,     // ashariya (float)
    KwHarf,         // harf (character)
    KwMantiqi,      // mantiqi (boolean)
    KwMustaqil,     // mustaqil (constant)

    KwAgar,         // agar (if)
    KwWarna,        // warna (else)
    KwJabtak,       // jabtak (while)
    KwKeLiye,       // ke_liye (for)
    KwWapas,        // wapas (return)

    KwSach,         // sach (true)
    KwJhoot,        // jhoot (false)

    // Identifiers and literals
    Identifier,
    IntegerLiteral,
    FloatLiteral,
    CharacterLiteral,

    // Arithmetic operators
    Plus,
    Minus,
    Star,
    Slash,
    Percent,

    // Assignment and comparison operators
    Assign,
    Equal,
    NotEqual,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,

    // Logical operators
    AndAnd,
    OrOr,
    Bang,

    // Increment and decrement
    PlusPlus,
    MinusMinus,

    // Delimiters
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Comma,
    Semicolon,

    // Special tokens
    EndOfFile,
    Invalid
};

// Represents one token
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};
