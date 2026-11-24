#include "lexer.h"

#include <cctype>
#include <utility>


bool Lexer::isAtEnd(size_t offset) const
{
    return position+offset >=source.size();
}

char Lexer::currentChar()
{
    if(isAtEnd())
        return '\0';

    return source[position];
}

char Lexer::peek(size_t offset) const
{
    if (isAtEnd(offset))
        return '\0';
    return source[position+offset];
}

char Lexer::next()
{
    if(isAtEnd())
        return '\0';
    char c=source[position++];
    if(c=='\n')
    {
        line++;
        character=1;
    }
    else
        character++;
    return c;
}


void Lexer::skipWhitespaceAndComments()
{
    while (!isAtEnd())
    {
        if(std::isspace(static_cast<unsigned char>(currentChar())))
        {
            next();
            continue;
        }
        if (currentChar()=='/' && peek() == '/')
        {
            while (!isAtEnd() && currentChar() != '\n')
                next();
            continue;
        }
        break;
    }
}


Token Lexer::scanKeywordIdentifier()
{
    size_t startLine = line;
    size_t startChar = character;
    std::string lexeme;

    while (!isAtEnd() && (std::isalnum(static_cast<unsigned char>(currentChar())) || currentChar() == '_'))
        lexeme+=next();

    auto type= keywords.find(lexeme);

    if (type != keywords.end())
        return {type->second,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
    return {TokenType::Identifier,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
}

Token Lexer::scanNumber()
{
    size_t startLine = line;
    size_t startChar = character;
    std::string lexeme;

    while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(currentChar())))
        lexeme+=next();

    TokenType type= TokenType::IntegerLiteral;
    if(currentChar()=='.' && std::isdigit(static_cast<unsigned char>(peek())))
    {
        type = TokenType::FloatLiteral;
        lexeme+=next();
        while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(currentChar())))
            lexeme+=next();
    }
    return {type,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
}

Token Lexer::scanCharacter()
{
    size_t startLine = line;
    size_t startChar = character;
    std::string lexeme;

    lexeme+=next();
    if(isAtEnd() || currentChar()=='\'' || currentChar()=='\n')
        return {TokenType::Invalid,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
    if (currentChar() == '\\')
    {
        lexeme+=next(); 
        if (isAtEnd())
            return {TokenType::Invalid,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
        lexeme+=next();
    }
    else
        lexeme+=next();

    if (currentChar()!='\'')
        return {TokenType::Invalid,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
    lexeme+=next();

    return {TokenType::CharacterLiteral,lexeme,static_cast<int>(startLine),static_cast<int>(startChar)};
}

Token Lexer::scanOperatorOrDelimiter()
{
    size_t startLine = line;
    size_t startChar = character;
    char c = next();

    auto match = [&](char e,TokenType single,TokenType duo)
    {
        if(currentChar()==e)
        {
            next();
            return Token{duo,std::string{c,e},static_cast<int>(startLine),static_cast<int>(startChar)};
        }
        return Token{single,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)};
    };

    switch (c)
    {
    case '+': return match('+',TokenType::Plus,TokenType::PlusPlus);
    case '-': return match('-',TokenType::Minus,TokenType::MinusMinus);
    case '*': return Token{TokenType::Star,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)};
    case '/': return Token{TokenType::Slash,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)};
    case '%': return Token{TokenType::Percent,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    
    case '=': return match('=',TokenType::Assign,TokenType::Equal);
    case '<': return match('=',TokenType::Less,TokenType::LessEqual);
    case '>': return match('=',TokenType::Greater,TokenType::GreaterEqual);
    case '!': return match('=',TokenType::Bang,TokenType::NotEqual);

    case '&': return match('&',TokenType::BitAnd,TokenType::AndAnd);
    case '|': return match('|',TokenType::BitOr,TokenType::OrOr);
    
    case '(': return Token{TokenType::LeftParen,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case ')': return Token{TokenType::RightParen,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case '{': return Token{TokenType::LeftBrace,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case '}': return Token{TokenType::RightBrace,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case '[': return Token{TokenType::LeftBrack,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case ']': return Token{TokenType::RightBrack,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 

    case ',': return Token{TokenType::Comma,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    case ';': return Token{TokenType::Semicolon,std::string{1,c},static_cast<int>(startLine),static_cast<int>(startChar)}; 
    
    default:
        return {TokenType::Invalid,std::string(1,c),static_cast<int>(startLine),static_cast<int>(startChar)};
    }

}

Lexer::Lexer(const std::string& sourceCode) : source(sourceCode) {}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (!isAtEnd())
    {
        skipWhitespaceAndComments();
        if(isAtEnd())
            break;
        char c = currentChar();
        if(std::isalpha(static_cast<unsigned char>(c)) || c=='_')
            tokens.push_back(scanKeywordIdentifier());
        else if(std::isdigit(static_cast<unsigned char>(c)))
            tokens.push_back(scanNumber());
        else if(c == '\'')
            tokens.push_back(scanCharacter());
        else
            tokens.push_back(scanOperatorOrDelimiter());

    }
    tokens.push_back({TokenType::EndOfFile,"",line,character});

    return tokens;
    
}