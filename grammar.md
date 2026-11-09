# SmartLang Grammar (EBNF)

## 1. Program Structure

Program          = { Declaration } ;

Declaration      = VariableDecl
                 | FunctionDecl ;

## 2. Variable Declarations

VariableDecl     = [ "const" ] Type Identifier
                   [ "=" Expression ] ";" ;

Type             = "int"
                 | "float"
                 | "char"
                 | "bool" ;

## 3. Function Declarations

FunctionDecl     = Type Identifier "(" [ Parameters ] ")"
                   Block ;

Parameters       = Parameter { "," Parameter } ;

Parameter        = Type Identifier ;

## 4. Blocks and Statements

Block            = "{" { Statement } "}" ;

Statement        = VariableDecl
                 | Assignment
                 | IfStatement
                 | WhileStatement
                 | ForStatement
                 | ReturnStatement
                 | Block ;

Assignment       = Identifier "=" Expression ";" ;

## 5. Conditional Statements

IfStatement      = "if" "(" Expression ")" Block
                   [ "else" ( IfStatement | Block ) ] ;

## 6. Loops

WhileStatement   = "while" "(" Expression ")" Block ;

ForStatement     = "for" "(" [ ForInit ] ";"
                   [ Expression ] ";"
                   [ ForUpdate ] ")" Block ;

ForInit          = VariableDeclNoSemi
                 | AssignmentNoSemi ;

ForUpdate        = AssignmentNoSemi ;

VariableDeclNoSemi = [ "const" ] Type Identifier
                     [ "=" Expression ] ;

AssignmentNoSemi = Identifier "=" Expression ;

## 7. Return Statement

ReturnStatement  = "return" [ Expression ] ";" ;

## 8. Expressions

Expression       = LogicalExpression ;

LogicalExpression = ComparisonExpression
                    { LogicalOperator ComparisonExpression } ;

ComparisonExpression = ArithmeticExpression
                       [ RelationalOperator ArithmeticExpression ] ;

ArithmeticExpression = Term { ("+" | "-") Term } ;

Term             = Factor { ("*" | "/" | "%") Factor } ;

Factor           = Number
                 | Character
                 | Boolean
                 | Identifier
                 | "(" Expression ")"
                 | UnaryOperator Factor ;

LogicalOperator  = "&&" | "||" ;

RelationalOperator = "==" | "!=" | "<" | ">"
                   | "<=" | ">=" ;

UnaryOperator    = "!" | "-" | "+" ;

## 9. Identifiers and Literals

Identifier       = Letter { Letter | Digit | "_" } ;

Number           = Integer | Float ;

Integer          = Digit { Digit } ;

Float            = Digit { Digit } "." Digit { Digit } ;

Character        = "'" CharacterValue "'" ;

Boolean          = "true" | "false" ;

## 10. Lexical Elements

Letter           = "a"..."z" | "A"..."Z" ;

Digit            = "0"..."9" ;
