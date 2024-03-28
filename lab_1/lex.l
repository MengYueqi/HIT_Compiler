%{
# include <stdio.h>
# include "parser.h"
# include "syntax.tab.h"
%}


digit [0-9]

%%

[\r\n]+
[ \t]+      
[+-]?0[xX][0-9a-fA-F]+|[+-]?0[0-7]*|[+-]?([1-9][0-9]*|0) {printf("INT\n"); return INT;}
[+-]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {printf("FLOAT\n"); return FLOAT;}
";"   {printf("SEMI\n"); return SEMI;}
","   {printf("COMMA\n"); return COMMA;}
"="   {printf("ASSIGNOP\n"); return ASSIGNOP;}
"+"   {printf("PLUS\n"); return PLUS;}
"-"   {printf("MINUS\n"); return MINUS;}
"*"   {printf("STAR\n"); return STAR;}
"/"   {printf("DIV\n"); return DIV;}
"&&"  {printf("AND\n"); return AND;}
"||"  {printf("OR\n"); return OR;}
"."   {printf("DOT\n"); return DOT;}
"!"   {printf("NOT\n"); return NOT;}
"("   {printf("LP\n"); return LP;}
")"   {printf("RP\n"); return RP;}
"["   {printf("LB\n"); return LB;}
"]"   {printf("RB\n"); return RB;}
"{"   {printf("LC\n"); return LC;}
"}"   {printf("RC\n"); return RC;}
"struct"   {printf("STRUCT\n"); return STRUCT;}
"return"   {printf("RETUEN\n"); return RETURN;}
"if"   {printf("IF\n"); return IF;}
"else"   {printf("ELSE\n"); return ELSE;}
"while"   {printf("WHILE\n"); return WHILE;}
.               { printf("Invalid character: %s\n", yytext); }

%%

// int main() {
//     while (yylex() != 0){}
//     return 0;
// }
