
#include "lexer.h"

#include <cctype>
#include <unordered_map>
#include <utility>

namespace {

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

bool isIdentifierStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c))
           || c == '_';
}

bool isIdentifierPart(char c) {
    return std::isalnum(static_cast<unsigned char>(c))
           || c == '_';
}

} // namespace

Lexer::Lexer(const std::string& sourceCode)
    : source(sourceCode) {
}

bool Lexer::isAtEnd() const {
    return position >= source.size();
}

char Lexer::currentChar() {
    if (isAtEnd()) {
        return '\0';
    }

    return source[position];
}

char Lexer::peek(size_t offset) const {
    if (position + offset >= source.size()) {
        return '\0';
    }

    return source[position + offset];
}

char Lexer::advance() {
    if (isAtEnd()) {
        return '\0';
    }

    char c = source[position++];

    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }

    return c;
}

Token Lexer::makeToken(
    TokenType type,
    const std::string& lexeme,
    int startLine,
    int startColumn
) {
    return {type, lexeme, startLine, startColumn};
}

// Skip spaces, tabs, newlines and comments.
void Lexer::skipWhitespaceAndComments() {

    while (!isAtEnd()) {

        char c = currentChar();

        if (std::isspace(static_cast<unsigned char>(c))) {
            advance();
            continue;
        }

        // Single-line comment
        if (c == '/' && peek() == '/') {

            while (!isAtEnd() && currentChar() != '\n') {
                advance();
            }

            continue;
        }

        break;
    }
}

// Recognize keywords and identifiers.
Token Lexer::scanIdentifier() {

    int startLine = line;
    int startColumn = column;

    std::string lexeme;

    while (!isAtEnd() && isIdentifierPart(currentChar())) {
        lexeme += advance();
    }

    auto it = keywords.find(lexeme);

    if (it != keywords.end()) {
        return makeToken(
            it->second, lexeme, startLine, startColumn
        );
    }

    return makeToken(
        TokenType::Identifier, lexeme, startLine, startColumn
    );
}

// Recognize integer and floating-point numbers.
Token Lexer::scanNumber() {

    int startLine = line;
    int startColumn = column;

    std::string lexeme;

    while (!isAtEnd() &&
           std::isdigit(static_cast<unsigned char>(currentChar()))) {
        lexeme += advance();
    }

    TokenType type = TokenType::IntegerLiteral;

    if (currentChar() == '.' &&
        std::isdigit(static_cast<unsigned char>(peek()))) {

        type = TokenType::FloatLiteral;

        lexeme += advance();

        while (!isAtEnd() &&
               std::isdigit(static_cast<unsigned char>(currentChar()))) {
            lexeme += advance();
        }
    }

    return makeToken(type, lexeme, startLine, startColumn);
}

// Recognize character literals such as 'A'.
Token Lexer::scanCharacter() {

    int startLine = line;
    int startColumn = column;

    std::string lexeme;

    lexeme += advance(); // Opening quote

    if (isAtEnd() || currentChar() == '\n' ||
        currentChar() == '\'') {

        return makeToken(
            TokenType::Invalid, lexeme, startLine, startColumn
        );
    }

    if (currentChar() == '\\') {
        lexeme += advance();

        if (isAtEnd()) {
            return makeToken(
                TokenType::Invalid, lexeme, startLine, startColumn
            );
        }

        lexeme += advance();
    } else {
        lexeme += advance();
    }

    if (currentChar() != '\'') {
        return makeToken(
            TokenType::Invalid, lexeme, startLine, startColumn
        );
    }

    lexeme += advance(); // Closing quote

    return makeToken(
        TokenType::CharacterLiteral,
        lexeme,
        startLine,
        startColumn
    );
}

// Recognize operators and delimiters.
Token Lexer::scanOperatorOrDelimiter() {

    int startLine = line;
    int startColumn = column;

    char c = advance();

    auto make = [&](TokenType type) {
        return makeToken(
            type,
            std::string(1, c),
            startLine,
            startColumn
        );
    };

    auto match = [&](char expected, TokenType type) {
        if (currentChar() == expected) {
            c = c; // First character already consumed
            advance();

            return makeToken(
                type,
                std::string() + c + expected,
                startLine,
                startColumn
            );
        }

        return makeToken(
            TokenType::Invalid,
            std::string(1, c),
            startLine,
            startColumn
        );
    };

    switch (c) {

        case '+':
            if (currentChar() == '+') {
                advance();
                return makeToken(TokenType::PlusPlus, "++",
                                 startLine, startColumn);
            }
            return make(TokenType::Plus);

        case '-':
            if (currentChar() == '-') {
                advance();
                return makeToken(TokenType::MinusMinus, "--",
                                 startLine, startColumn);
            }
            return make(TokenType::Minus);

        case '*': return make(TokenType::Star);
        case '/': return make(TokenType::Slash);
        case '%': return make(TokenType::Percent);

        case '=':
            if (currentChar() == '=') {
                advance();
                return makeToken(TokenType::Equal, "==",
                                 startLine, startColumn);
            }
            return make(TokenType::Assign);

        case '!':
            if (currentChar() == '=') {
                advance();
                return makeToken(TokenType::NotEqual, "!=",
                                 startLine, startColumn);
            }
            return make(TokenType::Bang);

        case '<':
            if (currentChar() == '=') {
                advance();
                return makeToken(TokenType::LessEqual, "<=",
                                 startLine, startColumn);
            }
            return make(TokenType::Less);

        case '>':
            if (currentChar() == '=') {
                advance();
                return makeToken(TokenType::GreaterEqual, ">=",
                                 startLine, startColumn);
            }
            return make(TokenType::Greater);

        case '&':
            if (currentChar() == '&') {
                advance();
                return makeToken(TokenType::AndAnd, "&&",
                                 startLine, startColumn);
            }
            return make(TokenType::Invalid);

        case '|':
            if (currentChar() == '|') {
                advance();
                return makeToken(TokenType::OrOr, "||",
                                 startLine, startColumn);
            }
            return make(TokenType::Invalid);

        case '(': return make(TokenType::LeftParen);
        case ')': return make(TokenType::RightParen);
        case '{': return make(TokenType::LeftBrace);
        case '}': return make(TokenType::RightBrace);
        case ',': return make(TokenType::Comma);
        case ';': return make(TokenType::Semicolon);

        default:
            return make(TokenType::Invalid);
    }
}

// Produce the complete token stream.
std::vector<Token> Lexer::tokenize() {

    std::vector<Token> tokens;

    while (!isAtEnd()) {

        skipWhitespaceAndComments();

        if (isAtEnd()) {
            break;
        }

        char c = currentChar();

        if (isIdentifierStart(c)) {
            tokens.push_back(scanIdentifier());
        }
        else if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(scanNumber());
        }
        else if (c == '\'') {
            tokens.push_back(scanCharacter());
        }
        else {
            tokens.push_back(scanOperatorOrDelimiter());
        }
    }

    tokens.push_back(
        makeToken(TokenType::EndOfFile, "", line, column)
    );

    return tokens;
}
