%{
# include <stdio.h>
# include <stdlib.h>
# include "parser.h"
%}

%union{
    Node node;
}

// 终结符集合
%token <TreeNode> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND
%token <TreeNode> OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE
%token <TreeNode> TIMES LPAREN NUMBER RPAREN DIVIDE
%%

expr : expr PLUS expr
     | expr SEMI expr
     | expr TIMES expr
     | expr DIVIDE expr
     | LPAREN expr RPAREN
     | NUMBER
     ;

%%

int yylex() {
    int c = getchar();
    if (c == EOF) return 0;
    if (c == '+') return PLUS;
    if (c == '-') return MINUS;
    if (c == '*') return TIMES;
    if (c == '/') return DIVIDE;
    if (c == '(') return LPAREN;
    if (c == ')') return RPAREN;
    if (c == 'a') {printf("This a!\n"); return NUMBER; }
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr "Error: %s\n" s);
}

int main() {
    while(yylex() != 0){
        yylex();
    }
    yyparse();
    return 0;
}
