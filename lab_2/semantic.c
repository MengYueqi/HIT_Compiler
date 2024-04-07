# include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
# include "semantic.h"
# include "parser.h"

// 设定头指针
symbol_node head = NULL;

// 创建变量表
static symbol_node _createSymbolNode(){
    symbol_node pointer = (symbol_node)malloc(sizeof(struct _SymbolNode));
    pointer->name = (char*)malloc(sizeof(35 * sizeof(char)));
    pointer->symbolType = NULL;
    pointer->next = NULL;
    return pointer;
}

// 初始化变量表
static void _initSymbolList(){
    head = _createSymbolNode();
}

// 打印变量表
static void _printSymbolList(symbol_node head){
    head = head->next;
    while (head){
        printf("Symbol: %s\n", head->name);
        head = head->next;
    }    
}

// 检查变量表中是否有相关数据
// 返回 1 代表在表中找到
static int _checkRecord(symbol_node node){
    symbol_node p = head->next;
    while (p){
        if (!strcmp(p->name, node->name)){
            return 1;
        }
        p = p->next;
    }
    return 0;
}

// 向符号表中添加记录
static void _addRecord(symbol_node sym_record){
    // 头插法添加
    sym_record->next = head->next;
    head->next = sym_record;
}

// 对 ExtDef 的分析
static void _ExtDef(Node root){

}

// 创建一个 type
static type _createType(Kind kind, int num, ...){
    type t = (type)malloc(sizeof(struct _Type));
    t->kind = kind;
    va_list tlist;
    va_start(tlist, num);
    switch(kind)
    {
        case BASIC:
            t->data.basic = va_arg(tlist, basic_type);
            // printf("创建基本类型: %d\n", t->u.basic);
            break;
        case ARRAY:
            t->data.arr.size = va_arg(tlist, int);
            t->data.arr.arr_type = va_arg(tlist, type);
            // printf("创建数组类型\n");
            break;
    }
    va_end(tlist);
    return t;
}

// 对 Specifier 的分析
static type _Specifier(Node root){
    if (!strcmp(root->child[0]->name, "TYPE")){
        if (!strcmp(root->child[0]->ID_NAME, "int")) {
            return _createType(BASIC, 1, INT);
        } else if (!strcmp(root->child[0]->ID_NAME, "float")){
            return _createType(BASIC, 1, FLOAT);
        }
    }
    return NULL;
}

// 对 DecList 的分析
static void _DecList(Node root, type var_type){
    _Dec(root->child[0], var_type);
    // 对多个变量的情况展开分析
    if (root->child[1]){
        _DecList(root->child[2], var_type);
    } else {
        return;
    }
}

// 对 Dec 的分析
static void _Dec(Node root, type var_type){
    _VarDec(root->child[0], var_type);
}

// 对 VarDec 的分析
static void _VarDec(Node root, type var_type){
    // 获取 ID 节点
    Node id = root->child[0];
    while (id->child[0]){
        id = id->child[0];
    }
    // 初始化 node 节点的数据并设置
    symbol_node temp = _createSymbolNode();
    strcpy(temp->name, id->ID_NAME);
    // 非数组元素
    if (!strcmp(root->child[0]->name, "ID")){
        temp->symbolType = var_type;
    } else {

    }
    // 向记录表中进行添加
    if (_checkRecord(temp)){
        printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", root->line, temp->name);
    } else {
        _addRecord(temp);
    }

}


// 对 Def 的分析
static void _Def(Node root){
    type Type = _Specifier(root->child[0]);
    _DecList(root->child[1], Type);
}

// 对 Exp 的分析
static void _Exp(Node root){
    /* Exp 对应的语法
    Exp -> Exp ASSIGNOP Exp
         | Exp AND Exp
         | Exp OR Exp
         | Exp RELOP Exp
         | Exp PLUS Exp
         | Exp MINUS Exp
         | Exp STAR Exp
         | Exp DIV Exp
         | LP Exp RP
         | MINUS Exp
         | NOT Exp
         | ID LP Args RP
         | ID LP RP
         | Exp LB Exp RB
         | Exp DOT ID
         | ID
         | INT
         | FLOAT
         */
    
    // 分析第 0 个 child
    Node child_0 = root->child[0];
    // 如果是 ID, INT, FLOAT
    if (!strcmp(child_0->name, "INT")){
        
    } else if (!strcmp(child_0->name, "FLOAT")){

    } else if (!strcmp(child_0->name, "ID")){
        /* code */
    }
}

// 向外暴露的语义分析器
void semantic(Node root){
    _initSymbolList();
    _semantic(root);
}

// 语义分析函数
static void _semantic(Node root){
    if (root == NULL){
        return;
    }
    // 根据不同的产生式部分进行分析 
    if (!strcmp(root->name, "ExtDef")){
        _ExtDef(root);
    } else if (!strcmp(root->name, "Def")){
        _Def(root);
    } else if (!strcmp(root->name, "Exp")){
        _Exp(root); 
    }
    for (int i = 0; i < root->num_child; i++){
        _semantic(root->child[i]);
    }
    // _printSymbolList(head); 
}