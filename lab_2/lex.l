%{
# include <stdio.h>
# include "parser.h"
# include "syntax.tab.h"
// void yyerror(char* msg) {
//       fprintf(stderr, "%s\n", msg);
//}
%}

%option yylineno

digit [0-9]

%%

[\r\n]+
[ \t]+      
[+-]?0[xX][0-9a-fA-F]+|[+-]?0[0-7]*|[+-]?([1-9][0-9]*|0) {yylval.node = newNode("INT", 0, yylineno, NULL); return INT;}
[+-]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {yylval.node = newNode("FLOAT", 0, yylineno, NULL); return FLOAT;}
";"   {yylval.node = newNode("SEMI", 0, yylineno, NULL); return SEMI;}
","   {yylval.node = newNode("COMMA", 0, yylineno, NULL); return COMMA;}
"="   {yylval.node = newNode("ASSIGNOP", 0, yylineno, NULL); return ASSIGNOP;}
"+"   {yylval.node = newNode("PLUS", 0, yylineno, NULL); return PLUS;}
"-"   {yylval.node = newNode("MINUS", 0, yylineno, NULL); return MINUS;}
"*"   {yylval.node = newNode("STAR", 0, yylineno, NULL); return STAR;}
"/"   {yylval.node = newNode("DIV", 0, yylineno, NULL); return DIV;}
"&&"  {yylval.node = newNode("AND", 0, yylineno, NULL); return AND;}
"||"  {yylval.node = newNode("OR", 0, yylineno, NULL); return OR;}
"."   {yylval.node = newNode("DOT", 0, yylineno, NULL); return DOT;}
"!"   {yylval.node = newNode("NOT", 0, yylineno, NULL); return NOT;}
"("   {yylval.node = newNode("LP", 0, yylineno, NULL); return LP;}
")"   {yylval.node = newNode("RP", 0, yylineno, NULL); return RP;}
"["   {yylval.node = newNode("LB", 0, yylineno, NULL); return LB;}
"]"   {yylval.node = newNode("RB", 0, yylineno, NULL); return RB;}
"{"   {yylval.node = newNode("LC", 0, yylineno, NULL); return LC;}
"}"   {yylval.node = newNode("RC", 0, yylineno, NULL); return RC;}
"struct" {yylval.node = newNode("STRUCT", 0, yylineno, NULL); return STRUCT;}
"return" {yylval.node = newNode("RETURN", 0, yylineno, NULL); return RETURN;}
"if"     {yylval.node = newNode("IF", 0, yylineno, NULL); return IF;}
"else"   {yylval.node = newNode("ELSE", 0, yylineno, NULL); return ELSE;}
"while"  {yylval.node = newNode("WHILE", 0, yylineno, NULL); return WHILE;}
"int"|"float" {yylval.node = newNode("TYPE", 0, yylineno, NULL); return TYPE;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.node = newNode("ID", 0, yylineno, NULL); return ID;}
.               {printf("Invalid character: %s\n", yytext); fault = 1;}

%%

// int main() {
//     while (yylex() != 0){}
//     return 0;
//}
