# include "parser.h"

// 字符串转换成 int 值的函数
int string_to_int(const char *str) {
    char *endptr;
    long int value = strtol(str, &endptr, 0);
    
    // 如果 endptr 等于 str，则转换失败
    if (endptr == str) {
        fprintf(stderr, "转换失败: 输入不是有效的整数字符串\n");
        return 0;
    }
    
    return (int)value;
}

// 将字符串转换为 float 值
double parse_float(const char *string) {
    // 使用标准库函数strtod将字符串转换为浮点数值
    return strtod(string, NULL);
}

Node newNode(char *name, int num_child, int line, Node childList[]){
    // 创建返回指针
    Node node = (Node)malloc(sizeof(struct TreeNode));
    // 将节点 name 和儿子数量进行初始化
    node->name = name;
    node->num_child = num_child;

    // 设置内部数值
    if (!strcmp(name, "INT")){
        node->INT_NUM = string_to_int(yytext);
    } else if (!strcmp(name, "FLOAT")) {
        node->FLOAT_NUM = parse_float(yytext);
    } else if (!strcmp(name, "TYPE")){
        char *str;
        str = (char *)malloc(sizeof(char) * 40);
        strcpy(str, yytext);
        node->ID_NAME = str;
    } else if (!strcmp(name, "ID")){
        char *str;
        str = (char *)malloc(sizeof(char) * 40);
        strcpy(str, yytext);
        node->ID_NAME = str;
    }
    

    // 设置关联孩子节点
    if (num_child != 0){
        node->line = childList[0]->line;
        for (int i = 0; i < num_child; i++){
            node->child[i] = childList[i];
        }
    } else {
        node->line = line;
        node->child[0] = NULL;
    }
    
    return node;
}

// 深度优先遍历函数
void DFS(Node root, int level) {
    if (root == NULL) {
        return;
    }
    
    // 打印当前节点信息
    if (root->num_child == 0){
        if (root->line != -1){
            for (int i = 0; i < level; i++){
                printf("  ");
            }
            printf("%s", root->name);
            if (!strcmp(root->name, "TYPE")){
                printf(": %s", root->ID_NAME);
            } else if (!strcmp(root->name, "ID")){
                printf(": %s", root->ID_NAME);
            } else if (!strcmp(root->name, "INT")){
                printf(": %d", root->INT_NUM);
            } else if (!strcmp(root->name, "FLOAT")){
                printf(": %f", root->FLOAT_NUM);
            }
            printf("\n");
        }
    } else {
        if (root->line !=-1){
            for (int i = 0; i < level; i++){
                printf("  ");
            }
            printf("%s (%d)\n", root->name, root->line);
        }
    }

    // 递归遍历子节点
    for (int i = 0; i < root->num_child; i++) {
        DFS(root->child[i], level+1);
    }
}

// int main(){
//     // 构建一棵树，这里简化为只有两层
//     Node leaf1 = newNode("Leaf1", 0, 10, NULL);
//     Node leaf2 = newNode("Leaf2", 0, 15, NULL);
//     Node leaf3 = newNode("Leaf3", 0, 20, NULL);

//     Node node1[2] = {leaf1, leaf2};
//     Node node2[1] = {leaf3};
//     Node intermediate1 = newNode("Intermediate1", 2, 5, node1);
//     Node intermediate2 = newNode("Intermediate2", 1, 12, node2);

//     Node node3[2] = {intermediate1, intermediate2};
//     Node root = newNode("Root", 2, 1, node3);

//     // 深度优先遍历树
//     DFS(root);

//     return 0;
// }