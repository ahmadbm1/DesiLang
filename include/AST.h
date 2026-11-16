
#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Token.h"

// =====================================
// BASE AST CLASSES
// =====================================

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct Expr : ASTNode {
    virtual ~Expr() = default;
};

struct Stmt : ASTNode {
    virtual ~Stmt() = default;
};

// Smart pointer aliases
using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;


// =====================================
// EXPRESSION NODES
// =====================================

// Literal: 20, 12.5, 'A', sach, jhoot
struct LiteralExpr : Expr {
    Token value;

    explicit LiteralExpr(Token value)
        : value(std::move(value)) {}
};

// Variable: age
struct VariableExpr : Expr {
    Token name;

    explicit VariableExpr(Token name)
        : name(std::move(name)) {}
};

// Unary: -age, !condition, ++age
struct UnaryExpr : Expr {
    Token op;
    ExprPtr right;

    UnaryExpr(Token op, ExprPtr right)
        : op(std::move(op)),
          right(std::move(right)) {}
};

// Binary: age + 10, age >= 18
struct BinaryExpr : Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;

    BinaryExpr(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right)) {}
};

// Assignment: age = 20
struct AssignmentExpr : Expr {
    Token name;
    ExprPtr value;

    AssignmentExpr(Token name, ExprPtr value)
        : name(std::move(name)),
          value(std::move(value)) {}
};

// Function call: calculate(10, 20)
struct CallExpr : Expr {
    Token callee;
    std::vector<ExprPtr> arguments;

    CallExpr(Token callee, std::vector<ExprPtr> arguments)
        : callee(std::move(callee)),
          arguments(std::move(arguments)) {}
};


// =====================================
// STATEMENT NODES
// =====================================

// Variable declaration: adad age = 20;
struct VarDeclStmt : Stmt {
    TokenType dataType;
    Token name;
    ExprPtr initializer;

    VarDeclStmt(
        TokenType dataType,
        Token name,
        ExprPtr initializer
    )
        : dataType(dataType),
          name(std::move(name)),
          initializer(std::move(initializer)) {}
};

// Expression statement: age = 20;
struct ExprStmt : Stmt {
    ExprPtr expression;

    explicit ExprStmt(ExprPtr expression)
        : expression(std::move(expression)) {}
};

// Block: { statements }
struct BlockStmt : Stmt {
    std::vector<StmtPtr> statements;
};

// If / Else statement
struct IfStmt : Stmt {
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;

    IfStmt(
        ExprPtr condition,
        StmtPtr thenBranch,
        StmtPtr elseBranch
    )
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
};

// While loop
struct WhileStmt : Stmt {
    ExprPtr condition;
    StmtPtr body;

    WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)),
          body(std::move(body)) {}
};

// Return statement
struct ReturnStmt : Stmt {
    ExprPtr value;

    explicit ReturnStmt(ExprPtr value)
        : value(std::move(value)) {}
};

// For loop
struct ForStmt : Stmt {
    StmtPtr initializer;
    ExprPtr condition;
    ExprPtr increment;
    StmtPtr body;
};
