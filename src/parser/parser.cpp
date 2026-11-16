
#include "parser.h"

#include <iostream>
#include <utility>

// =====================================
// Constructor
// =====================================

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {}


// =====================================
// TOKEN NAVIGATION
// =====================================

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::peek() const {
    return tokens[current < tokens.size()
        ? current
        : tokens.size() - 1];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) const {
    if (isAtEnd())
        return type == TokenType::EndOfFile;

    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            if (!isAtEnd())
                current++;

            return true;
        }
    }

    return false;
}

Token Parser::consume(
    TokenType type,
    const std::string& message
) {
    if (check(type)) {
        Token token = peek();

        if (!isAtEnd())
            current++;

        return token;
    }

    throw std::runtime_error(
        message + " at line " +
        std::to_string(peek().line)
    );
}


// =====================================
// MAIN PARSING FUNCTION
// =====================================

std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> statements;

    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        }
        catch (const std::exception& error) {
            std::cerr << "Parser error: "
                      << error.what() << '\n';

            synchronize();
        }
    }

    return statements;
}


// =====================================
// DECLARATIONS
// =====================================

StmtPtr Parser::declaration() {
    if (match({
        TokenType::KwAdad,
        TokenType::KwAshariya,
        TokenType::KwHarf,
        TokenType::KwMantiqi
    })) {
        return variableDeclaration();
    }

    return statement();
}


StmtPtr Parser::variableDeclaration() {
    TokenType dataType = previous().type;

    Token name = consume(
        TokenType::Identifier,
        "Expected variable name"
    );

    ExprPtr initializer = nullptr;

    if (match({TokenType::Assign})) {
        initializer = expression();
    }

    consume(
        TokenType::Semicolon,
        "Expected ';' after variable declaration"
    );

    return std::make_unique<VarDeclStmt>(
        dataType,
        name,
        std::move(initializer)
    );
}


// =====================================
// STATEMENTS
// =====================================

StmtPtr Parser::statement() {

    if (match({TokenType::LeftBrace}))
        return blockStatement();

    if (match({TokenType::KwAgar}))
        return ifStatement();

    if (match({TokenType::KwJabtak}))
        return whileStatement();

    if (match({TokenType::KwKeLiye}))
        return forStatement();

    if (match({TokenType::KwWapas}))
        return returnStatement();

    return expressionStatement();
}


// =====================================
// BLOCK STATEMENT
// =====================================

StmtPtr Parser::blockStatement() {
    auto block = std::make_unique<BlockStmt>();

    while (
        !check(TokenType::RightBrace) &&
        !isAtEnd()
    ) {
        block->statements.push_back(declaration());
    }

    consume(
        TokenType::RightBrace,
        "Expected '}' after block"
    );

    return block;
}


// =====================================
// IF / ELSE STATEMENT
// =====================================

StmtPtr Parser::ifStatement() {

    consume(
        TokenType::LeftParen,
        "Expected '(' after agar"
    );

    ExprPtr condition = expression();

    consume(
        TokenType::RightParen,
        "Expected ')' after condition"
    );

    StmtPtr thenBranch = statement();
    StmtPtr elseBranch = nullptr;

    if (match({TokenType::KwWarna})) {
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch)
    );
}


// =====================================
// WHILE LOOP
// =====================================

StmtPtr Parser::whileStatement() {

    consume(
        TokenType::LeftParen,
        "Expected '(' after jabtak"
    );

    ExprPtr condition = expression();

    consume(
        TokenType::RightParen,
        "Expected ')' after condition"
    );

    StmtPtr body = statement();

    return std::make_unique<WhileStmt>(
        std::move(condition),
        std::move(body)
    );
}


// =====================================
// FOR LOOP
// =====================================

StmtPtr Parser::forStatement() {

    consume(
        TokenType::LeftParen,
        "Expected '(' after ke_liye"
    );

    StmtPtr initializer = nullptr;

    if (match({TokenType::Semicolon})) {
        initializer = nullptr;
    }
    else if (match({
        TokenType::KwAdad,
        TokenType::KwAshariya,
        TokenType::KwHarf,
        TokenType::KwMantiqi
    })) {
        initializer = variableDeclaration();
    }
    else {
        initializer = expressionStatement();
    }

    ExprPtr condition = nullptr;

    if (!check(TokenType::Semicolon)) {
        condition = expression();
    }

    consume(
        TokenType::Semicolon,
        "Expected ';' after loop condition"
    );

    ExprPtr increment = nullptr;

    if (!check(TokenType::RightParen)) {
        increment = expression();
    }

    consume(
        TokenType::RightParen,
        "Expected ')' after for clauses"
    );

    StmtPtr body = statement();

    auto loop = std::make_unique<ForStmt>();

    loop->initializer = std::move(initializer);
    loop->condition = std::move(condition);
    loop->increment = std::move(increment);
    loop->body = std::move(body);

    return loop;
}


// =====================================
// RETURN STATEMENT
// =====================================

StmtPtr Parser::returnStatement() {

    ExprPtr value = nullptr;

    if (!check(TokenType::Semicolon)) {
        value = expression();
    }

    consume(
        TokenType::Semicolon,
        "Expected ';' after return value"
    );

    return std::make_unique<ReturnStmt>(
        std::move(value)
    );
}


// =====================================
// EXPRESSION STATEMENT
// =====================================

StmtPtr Parser::expressionStatement() {

    ExprPtr expr = expression();

    consume(
        TokenType::Semicolon,
        "Expected ';' after expression"
    );

    return std::make_unique<ExprStmt>(
        std::move(expr)
    );
}


// =====================================
// EXPRESSIONS
// =====================================

ExprPtr Parser::expression() {
    return assignment();
}


// Assignment: age = 20

ExprPtr Parser::assignment() {

    ExprPtr expr = logicalOr();

    if (match({TokenType::Assign})) {

        Token equals = previous();

        ExprPtr value = assignment();

        auto* variable =
            dynamic_cast<VariableExpr*>(expr.get());

        if (variable != nullptr) {
            Token name = variable->name;

            return std::make_unique<AssignmentExpr>(
                name,
                std::move(value)
            );
        }

        throw std::runtime_error(
            "Invalid assignment target at line " +
            std::to_string(equals.line)
        );
    }

    return expr;
}


// Logical OR: ||

ExprPtr Parser::logicalOr() {

    ExprPtr expr = logicalAnd();

    while (match({TokenType::OrOr})) {

        Token op = previous();

        ExprPtr right = logicalAnd();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Logical AND: &&

ExprPtr Parser::logicalAnd() {

    ExprPtr expr = equality();

    while (match({TokenType::AndAnd})) {

        Token op = previous();

        ExprPtr right = equality();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Equality: == !=

ExprPtr Parser::equality() {

    ExprPtr expr = comparison();

    while (match({
        TokenType::Equal,
        TokenType::NotEqual
    })) {

        Token op = previous();

        ExprPtr right = comparison();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Comparison: < > <= >=

ExprPtr Parser::comparison() {

    ExprPtr expr = term();

    while (match({
        TokenType::Less,
        TokenType::Greater,
        TokenType::LessEqual,
        TokenType::GreaterEqual
    })) {

        Token op = previous();

        ExprPtr right = term();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Addition and subtraction: + -

ExprPtr Parser::term() {

    ExprPtr expr = factor();

    while (match({
        TokenType::Plus,
        TokenType::Minus
    })) {

        Token op = previous();

        ExprPtr right = factor();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Multiplication, division and modulo: * / %

ExprPtr Parser::factor() {

    ExprPtr expr = unary();

    while (match({
        TokenType::Star,
        TokenType::Slash,
        TokenType::Percent
    })) {

        Token op = previous();

        ExprPtr right = unary();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}


// Unary: - ! ++ --

ExprPtr Parser::unary() {

    if (match({
        TokenType::Bang,
        TokenType::Minus,
        TokenType::PlusPlus,
        TokenType::MinusMinus
    })) {

        Token op = previous();

        ExprPtr right = unary();

        return std::make_unique<UnaryExpr>(
            op,
            std::move(right)
        );
    }

    return primary();
}


// =====================================
// PRIMARY EXPRESSIONS
// =====================================

ExprPtr Parser::primary() {

    if (match({
        TokenType::IntegerLiteral,
        TokenType::FloatLiteral,
        TokenType::CharacterLiteral,
        TokenType::KwSach,
        TokenType::KwJhoot
    })) {
        return std::make_unique<LiteralExpr>(
            previous()
        );
    }

    if (match({TokenType::Identifier})) {

        Token name = previous();

        // Function call
        if (match({TokenType::LeftParen})) {

            std::vector<ExprPtr> arguments;

            if (!check(TokenType::RightParen)) {

                do {
                    arguments.push_back(expression());

                } while (match({TokenType::Comma}));
            }

            consume(
                TokenType::RightParen,
                "Expected ')' after arguments"
            );

            return std::make_unique<CallExpr>(
                name,
                std::move(arguments)
            );
        }

        return std::make_unique<VariableExpr>(name);
    }

    if (match({TokenType::LeftParen})) {

        ExprPtr expr = expression();

        consume(
            TokenType::RightParen,
            "Expected ')' after expression"
        );

        return expr;
    }

    throw std::runtime_error(
        "Expected expression at line " +
        std::to_string(peek().line)
    );
}


// =====================================
// ERROR RECOVERY
// =====================================

void Parser::synchronize() {

    if (!isAtEnd())
        current++;

    while (!isAtEnd()) {

        if (previous().type == TokenType::Semicolon)
            return;

        switch (peek().type) {

            case TokenType::KwAdad:
            case TokenType::KwAshariya:
            case TokenType::KwAgar:
            case TokenType::KwJabtak:
            case TokenType::KwKeLiye:
            case TokenType::KwWapas:
                return;

            default:
                break;
        }

        current++;
    }
}
