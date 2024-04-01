# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# define MAXNUM 10
extern int yylineno;
extern char* yytext;
extern int yylex(void);
int fault;

struct TreeNode
{
    int line; // 行号
    char *name; // 节点名
    int num_child; // 子节点数量
    struct TreeNode *child[MAXNUM]; // 子节点指针数组
    // 节点内数据
    union{
        int INT_NUM;
        char *ID_NAME;
        double FLOAT_NUM;
    };
    
};

typedef struct TreeNode* Node;
Node nodeList[500];
Node newNode(char *name, int num_child, int line, Node childList[]);
void yyerror(char *msg);
extern int yylex(void);
void DFS(Node root, int level);