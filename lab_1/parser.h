# define MAXNUM 10

struct TreeNode
{
    int line; // 行号
    char *name; // 节点名
    int num_child; // 子节点数量
    struct TreeNode *child[MAXNUM]; // 子节点指针数组
    // 节点内数据
    union data
    {
        int INT;
        char *ID;
        float FLOAT;
    };
    
};

typedef struct TreeNode* Node;
void yyerror(char *msg);
extern int yylex(void);