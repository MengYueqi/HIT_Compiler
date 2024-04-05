# include "parser.h"
# include "semantic.h"

extern int yyparse();
extern void yyrestart(FILE *);

void yyerror(char *massage) {
    fprintf(stderr, "Error type B at Line %d: %s.\n", yylineno, massage);
}

int main(int argc, char** argv) {
    fault = 0;
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    semantic(root);
    return 0;
}