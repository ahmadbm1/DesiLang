#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Token.h"

struct ASTNode
{
    virtual ~ASTNode() =default;
};
struct Expression : ASTNode
{
    virtual ~Expression() = default;
};
struct Statement : ASTNode
{
    virtual ~Statement() = default;
};

using ExpPtr = std::unique_ptr<Expression>;
using StmPtr = std::unique_ptr<Statement>;

/*
literal = .5 , 1 , 'a' , sach, jhoot 
varaible = name
unary   = ++ , !a , -a 
binary  = a+10,a==10,a>10,a>=10
assignemnt a=10
call    a(10,10),a(10,a(10))
*/
struct literalExpr : Expression
{
    Token value;
    explicit literalExpr(Token _value) : value(std::move(_value)) {}
};

struct varExpr : Expression
{
    Token name;
    explicit varExpr(Token _name) : name(std::move(_name)) {}
};

struct unaryExpr : Expression
{
    Token op;
    ExpPtr operandRight;
    explicit unaryExpr(Token _op,ExpPtr _operandRight) : op(std::move(_op)) , operandRight(std::move(_operandRight)) {}
};

struct binExpr : Expression
{
    ExpPtr operandLeft;
    Token op;
    ExpPtr operandRight;
    explicit binExpr(ExpPtr _operandLeft,Token _op,ExpPtr _operandRight) : operandLeft(std::move(_operandLeft)) , op(std::move(_op)) , operandRight(std::move(_operandRight)) {}
};

struct assignExpr : Expression
{
    Token name;
    ExpPtr val;
    explicit assignExpr(Token _name,ExpPtr _val) : name(std::move(_name)) , val(std::move(_val)) {}
};

struct callExpr : Expression
{
    Token calle;
    std::vector<ExpPtr> arg;
    explicit callExpr(Token _calle,std::vector<ExpPtr> _arg) : calle(std::move(_calle)) , arg(std::move(_arg)) {}
};

/*
vardec  = adad a, 
expression  = wrap expression into statement : assignment,call
braces {}
if = if else
for = inititalize, condition, increment , body
while = condition,body
ret = val
*/

struct vardecStm : Statement{
    TokenType dataType;
    Token name;
    ExpPtr intz;
    vardecStm( TokenType _dataType, Token _name, ExpPtr _intz) : dataType(_dataType) , name(std::move(_name)), intz(std::move(_intz)) {}
};

 struct expStm: Statement {
    ExpPtr expr;
    explicit expStm(ExpPtr _expr) : expr(std::move(_expr)) {}
};

struct bracesStm : Statement {
    std::vector<StmPtr> stm;
    explicit bracesStm(std::vector<StmPtr> _stm) : stm(std::move(_stm)) {}
};

struct ifStm : Statement {
    ExpPtr condition;
    StmPtr thenWhat;
    StmPtr elseCondition;
    ifStm( ExpPtr _condition, StmPtr _thenWhat, StmPtr _elseCondition) : condition(std::move(_condition)) , thenWhat(std::move(_thenWhat)), elseCondition(std::move(_elseCondition)) {}
};

struct forStm : Statement {
    StmPtr initializer;
    ExpPtr condition;
    ExpPtr step;
    StmPtr body;
    forStm(StmPtr _initializer,ExpPtr _condition,ExpPtr _step, StmPtr _body) :initializer(std::move(_initializer)) , condition(std::move(_condition)) ,step(std::move(_step)) , body(std::move(_body)) {}
};

struct whileStm : Statement {
    ExpPtr condition;
    StmPtr body;
    whileStm(ExpPtr _condition, StmPtr _body) :condition(std::move(_condition)) , body(std::move(_body)) {}
};

struct retStm : Statement {
    ExpPtr val;
    retStm( ExpPtr _val) : val(std::move(_val)) {}
};