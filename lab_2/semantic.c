# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <stdlib.h>
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

// 检查变量表中是否有相关数据，有则返回对应指针，没有返回 NULL
static symbol_node _findRecord(symbol_node node){
    symbol_node p = head->next;
    while (p){
        if (!strcmp(p->name, node->name)){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 向符号表中添加记录
static void _addRecord(symbol_node sym_record){
    // 头插法添加
    sym_record->next = head->next;
    head->next = sym_record;
}

// 对 ExtDef 的分析
static void _ExtDef(Node root){
    // 对全局定义的符号的分析
    type t = _Specifier(root->child[0]);
    if (!strcmp(root->child[1]->name, "ExtDecList")){
        _ExtDefList(root->child[1], t);
    }
}

// 对 ExtDecList 进行分析
static void _ExtDefList(Node root, type var_type){
    _VarDec(root->child[0], var_type);
}

// 创建一个 type
static type _createType(Kind kind, int num, ...){
    type t = (type)malloc(sizeof(struct _Type));
    t->kind = kind;
    va_list tlist;
    va_start(tlist, num);
    switch(kind){
        // 处理 BASIC
        case BASIC:
            t->data.basic = va_arg(tlist, basic_type);
            break;
        // 处理 ARRAY
        case ARRAY:
            t->data.arr.size = va_arg(tlist, int);
            t->data.arr.arr_type = va_arg(tlist, type);
            break;
        // 处理 STRUCTURE
        case STRUCTURE:
           t->data.struct_pointer = va_arg(tlist, symbol_node);
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
    } else if(!strcmp(root->child[0]->name, "StructSpecifier")) {
        _StructSpecifier(root->child[0]);
    }
}

// 对 StructSpecifier 进行分析
static void _StructSpecifier(Node root){
    if (!strcmp(root->child[1]->name, "OptTag")){
        _OptTag(root->child[1]);
    } else if (!strcmp(root->child[1]->name, "Tag")){
        _Tag(root->child[1]);
    }
}

// 对 OptTag 的处理
static void _OptTag(Node root){
    if (!strcmp(root->child[0]->name, "ID")){
        symbol_node temp = _createSymbolNode();
        temp->name = root->child[0]->ID_NAME;
        temp->symbolType = _createType(STRUCTURE, 1, NULL);
        // 这里还没有写 structure 后面跟的属性值
        _addRecord(temp);
    } else{
        return;
    }
}

// 对 Tag 的处理
static void _Tag(Node root){
    symbol_node temp = _createSymbolNode();
    temp->name = root->child[0]->ID_NAME;
    temp->symbolType = _createType(STRUCTURE, 1, NULL);
    // 这里还没有写 structure 后面跟的属性值
    _addRecord(temp);
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
        // 数组元素的处理
        Node vardec_node = root;
        // 获取数组的类型和 size
        while (vardec_node->num_child != 1){
            temp->symbolType = _createType(ARRAY, 2, atoi(vardec_node->child[2]), var_type);
            vardec_node = vardec_node->child[0];
        }
    }
    // 向记录表中进行添加
    if (_findRecord(temp)){
        fault = 1;
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
static type _Exp(Node root){
    // 如果是 ID, INT, FLOAT
    if (!strcmp(root->child[0]->name, "INT")){
        return _createType(BASIC, 1, INT);
    } else if (!strcmp(root->child[0]->name, "FLOAT")){
        return _createType(BASIC, 1, FLOAT);
    } else if (!strcmp(root->child[0]->name, "ID")){
        // 对 ID 的分析
        // 这里注意的是，变量表中的 node 和树中的 node 是不一样的类型
        // 所以要转换一下
        symbol_node s = _createSymbolNode();
        s->name = root->child[0]->ID_NAME;
        if (!_findRecord(s)){
            fault = 1;
            printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", root->child[0]->line, root->child[0]->ID_NAME);
        } else {
            return _findRecord(s)->symbolType;
        }
    } else if (!strcmp(root->child[0]->name, "Exp")){
        // 对数组元素展开分析
        if (!strcmp(root->child[1]->name, "LB")){
            type t1 = _Exp(root->child[0]);
            type t2 = _Exp(root->child[2]);
            if (t1->kind != ARRAY){
                fault = 1;
                printf("Error type 10 at Line %d: \"%s\" is not an array.\n", root->child[0]->line, root->child[0]->child[0]->ID_NAME);
            } else if (t2->kind != BASIC || t2->data.basic != INT){
                fault = 1;
                printf("Error type 12 at Line %d: \"%s\" is not an integer.\n", root->child[0]->line, root->child[2]->child[0]->ID_NAME);
            }
        } else if (!strcmp(root->child[1]->name, "DOT")){
            // 对非结构体变量使用 '.' 进行审查
            type t = _Exp(root->child[0]);
            if (t->kind != STRUCTURE){
                fault = 1;
                printf("Error type 13 at Line %d: \"%s\" is not a struct type, cannot be accessed using DOT notation.\n", root->child[0]->line, root->child[0]->child[0]->ID_NAME);
            }
        } else if (!strcmp(root->child[1]->name, "ASSIGNOP")){
            // 先对不能为左值的数据进行审查
            if (!strcmp(root->child[0]->child[0]->name, "INT") || !strcmp(root->child[0]->child[0]->name, "FLOAT")){
                fault = 1;
                printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", root->child[0]->line);
            } else if (!strcmp(root->child[0]->child[0]->name, "ID")){
                // 对于有关 ID 的赋值操作
                type t1 = _Exp(root->child[0]);
                type t2 = _Exp(root->child[2]);
                if (t1->data.basic != t2->data.basic){
                    fault = 1;
                    printf("Error type 5 at Line %d: Type mismatched for assignment.\n", root->child[0]->line);
                }
            }
        } else {
            // 对其他运算操作进行审查
            type t1 = _Exp(root->child[0]);
            type t2 = _Exp(root->child[2]);
            if (t1->data.basic != t2->data.basic){
                fault = 1;
                printf("Error type 7 at Line %d: Type mismatched for operands.\n", root->child[0]->line);
            }
        }
    }
}

// 向外暴露的语义分析器
void semantic(Node root){
    _initSymbolList();
    _semantic(root);
    // _printSymbolList(head);
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