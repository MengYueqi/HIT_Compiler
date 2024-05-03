# include "parser.h"
# include "semantic.h"
# include "inter.h"

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
    symbol_node symTable = semantic(root);
    DFS(root, 0);
    interCodeList = newInterCodeList();
    symTableList = symTable;
    genInter(root);
    return 0;
}