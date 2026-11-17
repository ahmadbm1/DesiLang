
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

    Identifier,
    IntegerLiteral,
    FloatLiteral,
    CharacterLiteral,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,

    Assign,
    Equal,
    NotEqual,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,

    AndAnd,
    BitAnd,
    BitOr,
    OrOr,
    Bang,

    PlusPlus,
    MinusMinus,

    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBrack,
    RightBrack,
    Comma,
    Semicolon,

    EndOfFile,
    Invalid
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};
