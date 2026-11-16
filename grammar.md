# DESILANG Specification

## 1. Program Structure

Program          = { Declaration } ;

Declaration      = VariableDecl
                 | FunctionDecl ;

## 2. Data Types and Declarations

Type             = "adad"
                 | "ashariya"
                 | "harf"
                 | "mantiqi" ;

VariableDecl     = [ "pakka" ] Type Identifier
                   [ "=" Expression ] ";" ;

## 3. Functions

FunctionCall     = Identifier "(" [ Arguments ] ")" ;

Arguments        = Expression { "," Expression } ;

FunctionDecl     = Type Identifier "(" [ Parameters ] ")"
                   Block ;

Parameters       = Parameter { "," Parameter } ;

Parameter        = Type Identifier ;

## 4. Blocks and Statements

Block            = "{" { Statement } "}" ;

Statement        = VariableDecl
                 | Assignment
                 | FunctionCall ";"
                 | IfStatement
                 | WhileStatement
                 | ForStatement
                 | ReturnStatement
                 | Block ;

Assignment       = Identifier "=" Expression ";"
                 | Identifier "++" ";"
                 | Identifier "--" ";" ;

## 5. Conditional Statements

IfStatement      = "agar" "(" Expression ")" Block
                   [ "warna" ( IfStatement | Block ) ] ;

## 6. Loops

WhileStatement   = "jabtak" "(" Expression ")" Block ;

ForStatement     = "ke_liye" "(" [ ForInit ] ";"
                   [ Expression ] ";"
                   [ ForUpdate ] ")" Block ;

ForInit          = VariableDeclNoSemi
                 | AssignmentNoSemi ;

VariableDeclNoSemi = [ "pakka" ] Type Identifier
                     [ "=" Expression ] ;

AssignmentNoSemi = Identifier "=" Expression
                 | Identifier "++"
                 | Identifier "--" ;

ForUpdate        = AssignmentNoSemi ;

## 7. Return Statement

ReturnStatement  = "wapas" [ Expression ] ";" ;

## 8. Expressions and Operator Precedence

Expression       = LogicalOr ;

LogicalOr        = LogicalAnd { "||" LogicalAnd } ;

LogicalAnd       = Equality { "&&" Equality } ;

Equality         = Comparison { ("==" | "!=") Comparison } ;

Comparison       = Arithmetic { ("<" | ">" | "<=" | ">=")
                                Arithmetic } ;

Arithmetic       = Term { ("+" | "-") Term } ;

Term             = Unary { ("*" | "/" | "%") Unary } ;

Unary            = ("!" | "-" | "+") Unary
                 | Primary ;

Primary          = Number
                 | Character
                 | "sach"
                 | "jhoot"
                 | FunctionCall
                 | Identifier
                 | "(" Expression ")" ;

## 9. Lexical Elements

Identifier       = Letter { Letter | Digit | "_" } ;

Number           = Integer | Float ;

Integer          = Digit { Digit } ;

Float            = Digit { Digit } "." Digit { Digit } ;

Character        = "'" CharacterValue "'" ;

Letter           = "a"..."z" | "A"..."Z" ;

Digit            = "0"..."9" ;

## 10. Operators

Arithmetic       = "+" | "-" | "*" | "/" | "%" ;

Relational       = "==" | "!=" | "<" | ">"
                 | "<=" | ">=" ;

Logical          = "&&" | "||" | "!" ;

Increment        = "++" | "--" ;

AssignmentOp     = "=" ;

## 11. Delimiters

Delimiters       = "(" | ")" | "{" | "}"
                 | "," | ";" ;

## 12. Comments

SingleLineComment = "//" { AnyCharacter } ;
