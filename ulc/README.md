## μlc - micro language compiler

This is μlc, a compiler for a micro, small, language. I'm still
working on it, so expect it to be somewhat broken, or not work at all.

## grammar

```
Prog: FuncVarDecl ProgDecl

FuncVarDecl: Type IDENTIFIER VarDecl ';' FuncVarDecl
           | Type IDENTIFIER '=' Expr VarDecl ';' FuncVarDecl
           | Type IDENTIFIER[NUMBER] VarDecl ';' FuncVarDecl
           | Type IDENTIFIER '(' ParamList ')' Block FuncVarDecl
           | empty
;

ProgDecl: 'main' Block

VarDecl: ',' IDENTIFIER VarDecl
       | ',' IDENTIFIER '=' Expr VarDecl
       | ',' IDENTIFIER '[' NUMBER ']' VarDecl
       | empty
;

ParamList: ParamListCont
         | empty
;

ParamListCont: Type IDENTIFIER
             | Type IDENTIFIER ',' ParamListCont
             | Type IDENTIFIER '[' ']'
             | Type IDENTIFIER '[' ']' ',' ParamListCont
;

Block: '{' VarDeclList CommList '}'
     | '{' '}'
;

VarDeclList: Type IDENTIFIER VarDecl ';' VarDeclList
           | Type IDENTIFIER '=' Expr  VarDecl ';' VarDeclList
           | Type IDENTIFIER '[' NUMBER ']' VarDecl ';' VarDeclList
           | empty
;

Type: TYPE_NUM
    | TYPE_CHA
;

CommList: Comm
        | Comm CommList
;

Comm: ';'
    | Expr ';'
    | 'return' Expr ';'
    | 'read' LvalExpr ReadVars ';'
    | 'write' Expr ';'
    | 'write' STRING ';'
    | IfStmt
    | IfStmt 'else' Comm
    | 'while' '(' Expr ')' Comm
    | Block
;

ReadVars: ',' LvalExpr Readvars
        | empty
;

IfStmt: 'if' '(' Expr ')' Comm
;

Expr: AssignExpr
;

AssignExpr: CondExpr
          | LvalExpr '=' AssignExpr
;

CondExpr: OrExpr
        | OrExpr '?' Expr ':' CondExpr
;

OrExpr: AndExpr
      | OrExpr 'or' AndExpr
;

AndExpr: EqExpr
       | AndExpr 'and' EqExpr
;

EqExpr: IneqExpr
      | EqExpr '==' IneqExpr
      | EqExpr '!=' IneqExpr
;

IneqExpr: AddExpr
        | IneqExpr '<' AddExpr
        | IneqExpr '>' AddExpr
        | IneqExpr '<=' AddExpr
        | IneqExpr '>=' AddExpr
;

AddExpr: MultExpr
       | AddExpr '+' MultExpr
       | AddExpr '-' MultExpr
;

MultExpr: UnExpr
        | MultExpr '*' UnExpr
        | MultExpr '/' UnExpr
        | MultExpr '%' UnExpr
;

UnExpr: PrimExpr
      | '-' PrimExpr
      | '!' PrimExpr
;

LvalExpr: IDENTIFIER
        | IDENTIFIER '[' Expr ']'
;

PrimExpr: IDENTIFIER
        | IDENTIFIER '(' ExprList ')'
        | IDENTIFIER '[' Expr ']'
        | '(' Expr ')'
        | CHAR
        | NUMBER

ExprList: AssignExpr
        | ExprList ',' AssignExpr

```


## some useful references

- Flex and Bison manuals
- The virtual machine used here as well as the code generation module were adapted from
  * [Compiler Construction using Flex and Bison](http://research.microsoft.com/en-us/um/people/rgal/ar_language/external/compiler.pdf")
