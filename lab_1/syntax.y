%{
# include <stdio.h>
# include <stdlib.h>
# include "lex.yy.c"
%}

%union{
    Node node;
}

// 终结符集合
%token <TreeNode> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND
%token <TreeNode> OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE INT FLOAT
%token <TreeNode> TIMES LPAREN NUMBER RPAREN DIVIDE ID

// 终结符集合
%type <TreeNode> Program ExtDefList ExtDef Specifier ExtDecList FunDec VarDec
%type <TreeNode> StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Stmt CompSt
%type <TreeNode> Exp Dec Args

%%

// High-level Definitions
Program : ExtDefList
    ;
ExtDefList : ExtDef ExtDefList
    |
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;

// Specifiers
Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    |
    ;   
Tag : ID
    ;

// Declarators
VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;

// Statements
CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    |
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

// Local Definitions
DefList : Def DefList
    |
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;

// Expressions
Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%

