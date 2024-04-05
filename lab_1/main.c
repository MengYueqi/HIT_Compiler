# include "parser.h"
extern int yyparse();
extern void yyrestart(FILE *);


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
    fclose(f);
    DFS(root, 0);
    return 0;
}