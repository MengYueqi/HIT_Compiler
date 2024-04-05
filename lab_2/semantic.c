# include <stdio.h>
# include "semantic.h"
# include "parser.h"

void semantic(Node root){
    if (root == NULL){
        return;
    }
    
    printf("%s", root->name);
}