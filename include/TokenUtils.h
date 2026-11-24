
#pragma once

#include "Token.h"

inline const char* tokenTypeToString(TokenType type) {
    switch (type) {

        case TokenType::KwAdad: return "KwAdad";
        case TokenType::KwAshariya: return "KwAshariya";
        case TokenType::KwHarf: return "KwHarf";
        case TokenType::KwMantiqi: return "KwMantiqi";
        case TokenType::KwMustaqil: return "KwMustaqil";

        case TokenType::KwAgar: return "KwAgar";
        case TokenType::KwWarna: return "KwWarna";
        case TokenType::KwJabtak: return "KwJabtak";
        case TokenType::KwKeLiye: return "KwKeLiye";
        case TokenType::KwWapas: return "KwWapas";

        case TokenType::KwSach: return "KwSach";
        case TokenType::KwJhoot: return "KwJhoot";

        case TokenType::Identifier: return "Identifier";
        case TokenType::IntegerLiteral: return "IntegerLiteral";
        case TokenType::FloatLiteral: return "FloatLiteral";
        case TokenType::CharacterLiteral: return "CharacterLiteral";

        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
        case TokenType::Percent: return "Percent";

        case TokenType::Assign: return "Assign";
        case TokenType::Equal: return "Equal";
        case TokenType::NotEqual: return "NotEqual";
        case TokenType::Less: return "Less";
        case TokenType::Greater: return "Greater";
        case TokenType::LessEqual: return "LessEqual";
        case TokenType::GreaterEqual: return "GreaterEqual";

        case TokenType::AndAnd: return "AndAnd";
        case TokenType::OrOr: return "OrOr";
        case TokenType::BitAnd: return "BitAnd";
        case TokenType::BitOr: return "BitOr";
        case TokenType::Bang: return "Bang";

        case TokenType::PlusPlus: return "PlusPlus";
        case TokenType::MinusMinus: return "MinusMinus";

        case TokenType::LeftParen: return "LeftParen";
        case TokenType::RightParen: return "RightParen";
        case TokenType::LeftBrace: return "LeftBrace";
        case TokenType::RightBrace: return "RightBrace";
        case TokenType::LeftBrack: return "LeftBrack";
        case TokenType::RightBrack: return "RightBrack";
        case TokenType::Comma: return "Comma";
        case TokenType::Semicolon: return "Semicolon";

        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Invalid: return "Invalid";
    }

    return "Unknown";
}
