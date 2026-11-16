
#pragma once

#include <vector>
#include <initializer_list>
#include <stdexcept>

#include "Token.h"
#include "AST.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    // Token navigation
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    bool check(TokenType type) const;
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);

    // Declarations and statements
    StmtPtr declaration();
    StmtPtr statement();
    StmtPtr variableDeclaration();
    StmtPtr blockStatement();
    StmtPtr ifStatement();
    StmtPtr whileStatement();
    StmtPtr forStatement();
    StmtPtr returnStatement();
    StmtPtr expressionStatement();

    // Expressions, arranged by operator precedence
    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logicalOr();
    ExprPtr logicalAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr primary();

    void synchronize();

public:
    explicit Parser(const std::vector<Token>& tokens);

    std::vector<StmtPtr> parse();
};
