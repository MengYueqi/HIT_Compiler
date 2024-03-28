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
%token <TreeNode> TIMES LPAREN NUMBER RPAREN DIVIDE
%%

expr : expr PLUS expr
     | INT
     ;

%%

