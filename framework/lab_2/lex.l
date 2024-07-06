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
";"   {yylval.node = newNode("SEMI", 0, yylineno, NULL); return SEMI;}
","   {}
"="   {}

"//".* {}
"/*"  {}
.  {printf("Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); fault = 1;}

%%

// int main() {
//     while (yylex() != 0){}
//     return 0;
//}
