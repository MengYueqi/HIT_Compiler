# include "parser.h"

Node newNode(char *name, int num_child, int line, Node childList[]){
    // 创建返回指针
    Node node = (Node)malloc(sizeof(struct TreeNode));
    // 将节点 name 和儿子数量进行初始化
    node->name = name;
    node->num_child = num_child;

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
void DFS(Node root) {
    if (root == NULL) {
        return;
    }
    
    // 打印当前节点信息
    printf("Node Name: %s, Line: %d\n", root->name, root->line);

    // 递归遍历子节点
    for (int i = 0; i < root->num_child; i++) {
        DFS(root->child[i]);
    }
}

int main(){
    // 构建一棵树，这里简化为只有两层
    Node leaf1 = newNode("Leaf1", 0, 10, NULL);
    Node leaf2 = newNode("Leaf2", 0, 15, NULL);
    Node leaf3 = newNode("Leaf3", 0, 20, NULL);

    Node node1[2] = {leaf1, leaf2};
    Node node2[1] = {leaf3};
    Node intermediate1 = newNode("Intermediate1", 2, 5, node1);
    Node intermediate2 = newNode("Intermediate2", 1, 12, node2);

    Node node3[2] = {intermediate1, intermediate2};
    Node root = newNode("Root", 2, 1, node3);

    // 深度优先遍历树
    DFS(root);

    return 0;
}