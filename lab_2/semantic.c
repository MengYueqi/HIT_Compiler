# include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
# include "semantic.h"
# include "parser.h"

// 设定头指针
symbol_node head = NULL;

// 创建变量表
symbol_node _createSymbolList(){
    symbol_node pointer = (symbol_node)malloc(sizeof(struct _SymbolNode));
    pointer->name = (char*)malloc(sizeof(35 * sizeof(char)));
    pointer->type = NULL;
    pointer->next = NULL;
    return pointer;
}

// 初始化变量表
void _initSymbolList(){
    head = _createSymbolList();
}

// 打印变量表
void _printSymbolList(symbol_node head){
    head = head->next;
    while (head){
        printf("Symbol: %s, Type: %s", head->name, head->type);
        head = head->next;
    }    
}

// 向符号表中添加记录
void _addRecord(symbol_node sym_record){
    // 头插法添加
    sym_record->next = head->next;
    head->next = sym_record;
}

// 对 ExtDef 的分析
void _ExtDef(Node root){

}

// 创建一个 type
type createType(Kind kind, int num, ...)
{
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
type _Specifier(Node root){
    if (!strcmp(root->child[0]->name, "TYPE")){
        if (!strcmp(root->child[0]->ID_NAME, "int")) {
            return createType(BASIC, 1, INT);
        } else if (!strcmp(root->child[0]->ID_NAME, "float")){
            return createType(BASIC, 1, FLOAT);
        }
    }
    return NULL;
}

// 对 DecList 的分析
void _DecList(Node root, type Type){
    
}

// 对 Def 的分析
void _Def(Node root){
    type Type = _Specifier(root->child[0]);


}

// 对 Exp 的分析
void _Exp(Node root){
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
        printf("%f", child_0->FLOAT_NUM);
    } else if (!strcmp(child_0->name, "ID")){
        /* code */
    }
}

void semantic(Node root){
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
        semantic(root->child[i]);
    }
    
}