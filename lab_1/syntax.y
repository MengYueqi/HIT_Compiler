%{
# include <stdio.h>
# include <stdlib.h>
# include "parser.h"
%}

%union{
    Node node;
}

// 终结符集合
%token <node> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND
%token <node> OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE INT FLOAT
%token <node> TIMES LPAREN NUMBER RPAREN DIVIDE ID

// 终结符集合
%type <node> Program ExtDefList ExtDef Specifier ExtDecList FunDec VarDec
%type <node> StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Stmt CompSt
%type <node> Exp Dec Args Def DecList

%%

// High-level Definitions
Program : ExtDefList {Node child[1] = {$1}; $$ = newNode("Program", 1, yylineno, child); DFS($$)}
    ;
ExtDefList : ExtDef ExtDefList {Node child[2] = {$1, $2}; $$ = newNode("ExtDefList", 2, yylineno, child);}
    |
    ;
ExtDef : Specifier ExtDecList SEMI {Node child[3] = {$1, $2, $3}; $$= newNode("ExtDef", 3, yylineno, child);}
    | Specifier SEMI {Node child[2] = {$1, $2}; $$= newNode("ExtDef", 2, yylineno, child);}
    | Specifier FunDec CompSt {Node child[3] = {$1, $2, $3}; $$= newNode("ExtDef", 3, yylineno, child);}
    ;
ExtDecList : VarDec {Node child[1] = {$1}; $$ = newNode("ExtDecList", 1, yylineno, child);}
    | VarDec COMMA ExtDecList {Node child[3] = {$1, $2, $3}; $$ = newNode("ExtDecList", 3, yylineno, child);}
    ;

// Specifiers
Specifier : TYPE {Node child[1] = {$1}; $$ = newNode("Specifier", 1, yylineno, child);}
    | StructSpecifier {Node child[1] = {$1}; $$ = newNode("Specifier", 1, yylineno, child);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {Node child[5] = {$1, $2, $3, $4, $5}; $$ = newNode("StructSpecifier", 5, yylineno, child);}
    | STRUCT Tag {Node child[2] = {$1, $2}; $$ = newNode("StructSpecifier", 2, yylineno, child);}
    ;
OptTag : ID {Node child[1] = {$1}; $$ = newNode("OptTag", 1, yylineno, child);}
    | /* empty */ 
    ;   
Tag : ID {Node child[1] = {$1}; $$ = newNode("Tag", 1, yylineno, child);}
    ;

// Declarators
VarDec : ID {Node child[1] = {$1}; $$ = newNode("VarDec", 1, yylineno, child);}
    | VarDec LB INT RB {Node child[4] = {$1, $2, $3, $4}; $$ = newNode("VarDec", 4, yylineno, child);}
    ;
FunDec : ID LP VarList RP {Node child[4] = {$1, $2, $3, $4}; $$ = newNode("FunDec", 4, yylineno, child);}
    | ID LP RP {Node child[3] = {$1, $2, $3}; $$ = newNode("FunDec", 3, yylineno, child);}
    ;
VarList : ParamDec COMMA VarList {Node child[3] = {$1, $2, $3}; $$ = newNode("VarList", 3, yylineno, child);}
    | ParamDec {Node child[1] = {$1}; $$ = newNode("VarList", 1, yylineno, child);}
    ;
ParamDec : Specifier VarDec {Node child[2] = {$1, $2}; $$ = newNode("ParamDec", 2, yylineno, child);}
    ;

// Statements
CompSt : LC DefList StmtList RC {Node child[4] = {$1, $2, $3, $4}; $$ = newNode("CompSt", 4, yylineno, child);}
    ;
StmtList : Stmt StmtList {Node child[2] = {$1, $2}; $$ = newNode("StmtList", 2, yylineno, child);}
    | /* empty */
    ;
Stmt : Exp SEMI {Node child[2] = {$1, $2}; $$ = newNode("Stmt", 2, yylineno, child);}
    | CompSt {Node child[1] = {$1}; $$ = newNode("Stmt", 1, yylineno, child);}
    | RETURN Exp SEMI {Node child[3] = {$1, $2, $3}; $$ = newNode("Stmt", 3, yylineno, child);}
    | IF LP Exp RP Stmt {Node child[5] = {$1, $2, $3, $4, $5}; $$ = newNode("Stmt", 5, yylineno, child);}
    | IF LP Exp RP Stmt ELSE Stmt {Node child[7] = {$1, $2, $3, $4, $5, $6, $7}; $$ = newNode("Stmt", 7, yylineno, child);}
    | WHILE LP Exp RP Stmt {Node child[5] = {$1, $2, $3, $4, $5}; $$ = newNode("Stmt", 5, yylineno, child);}
    ;

// Local Definitions
DefList : Def DefList {Node child[2] = {$1, $2}; $$ = newNode("DefList", 2, yylineno, child);}
    | /* empty */
    ;
Def : Specifier DecList SEMI {Node child[3] = {$1, $2, $3}; $$ = newNode("Def", 3, yylineno, child);}
    ;
DecList : Dec {Node child[1] = {$1}; $$ = newNode("DecList", 1, yylineno, child);}
    | Dec COMMA DecList {Node child[3] = {$1, $2, $3}; $$ = newNode("DecList", 3, yylineno, child);}
    ;
Dec : VarDec {Node child[1] = {$1}; $$ = newNode("Dec", 1, yylineno, child);}
    | VarDec ASSIGNOP Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Dec", 3, yylineno, child);}
    ;

// Expressions
Exp : Exp ASSIGNOP Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp AND Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp OR Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp RELOP Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp PLUS Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp MINUS Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp STAR Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp DIV Exp {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | LP Exp RP {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | MINUS Exp {Node child[2] = {$1, $2}; $$ = newNode("Exp", 2, yylineno, child);}
    | NOT Exp {Node child[2] = {$1, $2}; $$ = newNode("Exp", 2, yylineno, child);}
    | ID LP Args RP {Node child[4] = {$1, $2, $3, $4}; $$ = newNode("Exp", 4, yylineno, child);}
    | ID LP RP {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | Exp LB Exp RB {Node child[4] = {$1, $2, $3, $4}; $$ = newNode("Exp", 4, yylineno, child);}
    | Exp DOT ID {Node child[3] = {$1, $2, $3}; $$ = newNode("Exp", 3, yylineno, child);}
    | ID {Node child[1] = {$1}; $$ = newNode("Exp", 1, yylineno, child);}
    | INT {Node child[1] = {$1}; $$ = newNode("Exp", 1, yylineno, child);}
    | FLOAT {Node child[1] = {$1}; $$ = newNode("Exp", 1, yylineno, child);}
    ;

Args : Exp COMMA Args
    | Exp
    ;

%%

