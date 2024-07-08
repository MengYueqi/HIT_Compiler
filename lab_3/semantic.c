# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <stdlib.h>
# include "semantic.h"
# include "parser.h"

// 设定头指针
symbol_node head = NULL;

// 创建变量表
symbol_node createSymbolNode(){
    symbol_node pointer = (symbol_node)malloc(sizeof(struct _SymbolNode));
    pointer->name = (char*)malloc(sizeof(35 * sizeof(char)));
    pointer->symbolType = NULL;
    pointer->next = NULL;
    return pointer;
}

// 初始化变量表
static inline void _initSymbolList(){
    head = createSymbolNode();
    symbol_node read = createSymbolNode();
    read->name = "read";
    printf("Here\n");
    read->symbolType = _createType(FUNCTION, 2, NULL, NULL);
    symbol_node write = createSymbolNode();
    write->name = "write";
    write->symbolType = _createType(FUNCTION, 2, NULL, NULL);
    _addRecord(read);
    _addRecord(write);
}

// 打印变量表
void printSymbolList(symbol_node head){
    head = head->next;
    while (head){
        printf("Symbol: %s\n", head->name);
        head = head->next;
    }    
}

// 检查变量表中是否有相关数据，有则返回对应指针，没有返回 NULL
symbol_node findRecord(symbol_node head, symbol_node node){
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
static inline void _addRecord(symbol_node sym_record){
    // 头插法添加
    sym_record->next = head->next;
    head->next = sym_record;
}

// 对 ExtDef 的分析
static inline void _ExtDef(Node root){
    // 对全局定义的符号的分析
    type t = _Specifier(root->child[0]);
    // 如果是结构体类型，特别对待一下
    // 结构体就单独处理了，不把它和其他混在一起，要不结构体内的变量也被添加到变量表里
    if (t->kind == STRUCTURE){
        symbol_node temp = createSymbolNode();
        temp->name = root->child[0]->name;
        // 这里要判断是不是结构体初次定义，如果是的话不能报错
        // 这里直接判断前面有没有相关结构体定义，儿子数不是 5 说明不是定义的结构体
        if (!findRecord(head, temp) && root->child[0]->child[0]->num_child != 5){
            fault = 1;
            printf("Error type 17 at Line %d: variables \"%s\" cannot be defined using undefined structs.\n", root->child[0]->line, root->child[1]->child[0]->child[0]->child[0]->ID_NAME);
            return;
        }
    } else if (!strcmp(root->child[1]->name, "ExtDecList")){
        _ExtDefList(root->child[1], t);
    } else if (!strcmp(root->child[1]->name, "FunDec")){
        _FuncDec(root->child[1], t);
        type return_type = _Specifier(root->child[0]);
    }
}

// 对 CompSt 的分析
static inline type _CompSt(Node root){
    return _StmtList(root->child[2]);
}

// 对 StmtList 的分析
static inline type _StmtList(Node root){
    while (1){
        // 获取最后一条语句
        if (root->child[1]->num_child != 2){
            // 将 root 更新到 Stmt，即最后一条语句
            if (root->child[0]->num_child == 3 && _Exp(root->child[0]->child[1])){
                return _Exp(root->child[0]->child[1]);
            } else{
                return NULL;
            }
        }
        root = root->child[1];
    }
}

// 对 FunDec 的分析
static inline void _FuncDec(Node root, type return_type){
    symbol_node temp = createSymbolNode();
    temp->name = root->child[0]->ID_NAME;
    temp->symbolType = _createType(FUNCTION, 2, return_type, NULL);
    if (findRecord(head, temp)){
        fault = 1;
        printf("Error type 4 at Line %d: The function name \"%s\" is duplicated.\n", root->child[0]->line, root->child[0]->ID_NAME);
    }
    _addRecord(temp);
    if (root->num_child == 4){
        _VarList(root->child[2]);
    }
}

// 对 VarList 的分析
static inline void _VarList(Node root){
    while(root->num_child != 1){
        _ParamDec(root->child[0]);
        root = root->child[2];
    }
    _ParamDec(root->child[0]);
}

// 对 ParamDec 的分析
static inline void _ParamDec(Node root){
    type t = _Specifier(root->child[0]);
    _VarDec(root->child[1], t);
}

// 对 ExtDecList 进行分析
static inline void _ExtDefList(Node root, type var_type){
    _VarDec(root->child[0], var_type);
}

// 创建一个 type
static inline type _createType(Kind kind, int num, ...){
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
        // 对函数进行创建，第一个量为返回值，第二个量为参数值
        case FUNCTION:
            t->data.func_type.return_type = va_arg(tlist, type);
            t->data.func_type.par_type = va_arg(tlist, symbol_node);
    }
    va_end(tlist);
    return t;
}

// 对 Specifier 的分析
static inline type _Specifier(Node root){
    if (!strcmp(root->child[0]->name, "TYPE")){
        if (!strcmp(root->child[0]->ID_NAME, "int")) {
            return _createType(BASIC, 1, INT);
        } else if (!strcmp(root->child[0]->ID_NAME, "float")){
            return _createType(BASIC, 1, FLOAT);
        }
    } else if(!strcmp(root->child[0]->name, "StructSpecifier")) {
        _StructSpecifier(root->child[0]);
        return _createType(STRUCTURE, 1, NULL);
    }
}

// 对 StructSpecifier 进行分析
static inline void _StructSpecifier(Node root){
    if (!strcmp(root->child[1]->name, "OptTag")){
        symbol_node temp_head = _DefList(root->child[3]);
        if (_hasDuplicateName(temp_head)){
            fault = 1;
            printf("Error type 15 at Line %d: struct \"%s\" contains duplicate internal variable definitions.\n", root->child[0]->line, root->child[1]->child[0]->ID_NAME);
            return;
        } else {
            _OptTag(root->child[1], temp_head);
        }
    } else if (!strcmp(root->child[1]->name, "Tag")){
        _Tag(root->child[1]);
    }
}

// 判断是否有相同的数据
static inline int _hasDuplicateName(symbol_node head){
    if (head == NULL || head->next == NULL){
        // 如果链表为空或者只有一个节点，则不可能存在重复名称的节点
        return 0;
    }
    symbol_node current = head->next;
    // 遍历链表
    while (current != NULL) {
        symbol_node runner = current->next;
        // 在当前节点之后查找是否有相同名称的节点
        while (runner != NULL) {
            // 如果找到相同名称的节点，则返回 true
            if (!strcmp(current->name, runner->name)) {
                return 1;
            }
            runner = runner->next;
        }
        // 移动到链表的下一个节点
        current = current->next;
    }
    // 如果没有找到相同名称的节点，则返回 false
    return 0;
}


// 对 DefList 的处理
static inline symbol_node _DefList(Node root){
    // 获取 Def
    symbol_node head_pointer = createSymbolNode();
    head_pointer->next = NULL;
    // 如果一个儿子都没有，直接返回，不然会出空指针异常
    if (root->num_child == 0){
        return head_pointer;
    }
    while (1){
        symbol_node temp = createSymbolNode();
        type t = _Specifier(root->child[0]->child[0]);
        temp->symbolType = t;
        // 如果出现结构体内就初始化的，报错
        if(root->child[0]->child[1]->child[0]->num_child != 1){
            fault = 1;
            printf("Error type 16 at Line %d: The struct \"%s\" contains initialized internal variable definitions.\n", root->child[0]->line, root->child[0]->child[1]->child[0]->child[0]->child[0]->ID_NAME);
            return NULL;
        }
        temp->name = root->child[0]->child[1]->child[0]->child[0]->child[0]->ID_NAME;
        // 头插法构建
        temp->next = head_pointer->next;
        head_pointer->next = temp;
        root = root->child[1];
        if (root->num_child == 0){
            break;
        }
    }
    return head_pointer;
}

// 对 OptTag 的处理
static inline void _OptTag(Node root, symbol_node var){
    if (!strcmp(root->child[0]->name, "ID")){
        symbol_node temp = createSymbolNode();
        temp->name = root->child[0]->ID_NAME;
        temp->symbolType = _createType(STRUCTURE, 1, var);
        if (findRecord(head, temp)){
            // 对结构体重复定义进行审查
            fault = 1;
            printf("Error type 16 at Line %d: The struct \"%s\" has been previously defined and cannot be redefined.\n", root->child[0]->line, root->child[0]->ID_NAME);
            return;
        }
        // 这里还没有写 structure 后面跟的属性值
        temp->symbolType->data.struct_pointer = var;
        _addRecord(temp);
    } else{
        return;
    }
}

// 对 Tag 的处理
static inline void _Tag(Node root){
    symbol_node temp = createSymbolNode();
    temp->name = root->child[0]->ID_NAME;
    temp->symbolType = _createType(STRUCTURE, 1, NULL);
    // 这里还没有写 structure 后面跟的属性值
    _addRecord(temp);
}

// 对 DecList 的分析
static inline void _DecList(Node root, type var_type){
    _Dec(root->child[0], var_type);
    // 对多个变量的情况展开分析
    if (root->child[1]){
        _DecList(root->child[2], var_type);
    } else {
        return;
    }
}

// 对 Dec 的分析
static inline void _Dec(Node root, type var_type){
    _VarDec(root->child[0], var_type);
}

// 对 VarDec 的分析
static inline void _VarDec(Node root, type var_type){
    // 获取 ID 节点
    Node id = root->child[0];
    while (id->child[0]){
        id = id->child[0];
    }
    // 初始化 node 节点的数据并设置
    symbol_node temp = createSymbolNode();
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
    if (findRecord(head, temp)){
        fault = 1;
        printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", root->line, temp->name);
    } else {
        _addRecord(temp);
    }
}


// 对 Def 的分析
static inline void _Def(Node root){
    type Type = _Specifier(root->child[0]);
    _DecList(root->child[1], Type);
}

// 对 Exp 的分析
static inline type _Exp(Node root){
    // 如果是 ID, INT, FLOAT
    if (!strcmp(root->child[0]->name, "INT")){
        return _createType(BASIC, 1, INT);
    } else if (!strcmp(root->child[0]->name, "FLOAT")){
        return _createType(BASIC, 1, FLOAT);
    } else if (!strcmp(root->child[0]->name, "ID")){
        if (root->num_child == 1){
            // 对 ID 的分析
            // 这里注意的是，变量表中的 node 和树中的 node 是不一样的类型
            // 所以要转换一下
            symbol_node s = createSymbolNode();
            s->name = root->child[0]->ID_NAME;
            if (!findRecord(head, s)){
                fault = 1;
                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", root->child[0]->line, root->child[0]->ID_NAME);
                return NULL;
            } else {
                return findRecord(head, s)->symbolType;
            }
        } else {
            // 大于一个儿子，判断是函数调用
            symbol_node s = createSymbolNode();
            s->name = root->child[0]->ID_NAME;
            if (!findRecord(head, s)){
                fault = 1;
                // 当调用一个函数没有定义的时候，报错
                printf("Error type 2 at Line %d: \"%s\" is undefined and cannot be invoked.\n", root->child[0]->line, root->child[0]->ID_NAME);
            } else if (findRecord(head, s)->symbolType->kind != FUNCTION){
                // 当调用的变量不是函数类型的时候，报错
                fault = 1;
                printf("Error type 11 at Line %d: symbol \"%s\" is not a function and thus cannot be called as one.\n", root->child[0]->line, root->child[0]->ID_NAME);
            } else{
                // TODO:这个地方应该输入是 symbol_node，但现在查出来的是 Type，之后改一下
                // _Args(root->child[2], _Exp(root->child[0]));
            }
        }
    } else if (!strcmp(root->child[0]->name, "Exp")){
        // 对数组元素展开分析
        if (!strcmp(root->child[1]->name, "LB")){
            type t1 = _Exp(root->child[0]);
            type t2 = _Exp(root->child[2]);
            if (!t1 || !t2){
                return NULL;
            }
            if (t1->kind != ARRAY){
                fault = 1;
                printf("Error type 10 at Line %d: \"%s\" is not an array.\n", root->child[0]->line, root->child[0]->child[0]->ID_NAME);
            } else if (t2->kind != BASIC || t2->data.basic != INT){
                fault = 1;
                printf("Error type 12 at Line %d: \"%f\" is not an integer.\n", root->child[0]->line, root->child[2]->child[0]->FLOAT_NUM);
            }
        } else if (!strcmp(root->child[1]->name, "DOT")){
            // 对非结构体变量使用 '.' 进行审查
            type t = _Exp(root->child[0]);
            if (t->kind != STRUCTURE){
                fault = 1;
                printf("Error type 13 at Line %d: \"%s\" is not a struct type, cannot be accessed using DOT notation.\n", root->child[0]->line, root->child[0]->child[0]->ID_NAME);
            } else{
                // 这个地方写的好像有点问题
                return NULL;
                symbol_node temp = createSymbolNode();
                temp->name = root->child[0]->child[0]->ID_NAME;
                // 获取结构体对应的节点
                symbol_node stru = findRecord(head, temp);
                // 将 temp 改为目标 ID
                temp->name = root->child[2]->ID_NAME;
                if (!findRecord(stru->symbolType->data.struct_pointer, temp)){
                    fault = 1;
                    printf("Error type 14 at Line %d: cannot access fields of an undefined struct.\n", root->child[1]->line);
                }
            }
        } else if (!strcmp(root->child[1]->name, "ASSIGNOP")){
            // 对于有关 ID 的赋值操作
            type t1 = _Exp(root->child[0]);
            type t2 = _Exp(root->child[2]);
            // 先对不能为左值的数据进行审查
            if (!strcmp(root->child[0]->child[0]->name, "INT") || !strcmp(root->child[0]->child[0]->name, "FLOAT")){
                fault = 1;
                printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", root->child[0]->line);
            } else if (!strcmp(root->child[0]->child[0]->name, "ID")){
                if (!t1 || !t2){
                    return NULL;
                }
                if (t1->data.basic != t2->data.basic){
                    fault = 1;
                    printf("Error type 5 at Line %d: Type mismatched for assignment.\n", root->child[0]->line);
                }
            }
        } else {
            // 对其他运算操作进行审查
            type t1 = _Exp(root->child[0]);
            type t2 = _Exp(root->child[2]);
            if (!t1 || !t2){
                return NULL;
            }
            if (t1->data.basic != t2->data.basic){
                fault = 1;
                printf("Error type 7 at Line %d: Type mismatched for operands.\n", root->child[0]->line);
            }
        }
    }
}

// 对 Args 的分析
static inline void _Args(Node root, symbol_node func_symbol){
    // type type_temp = func_symbol->symbolType;
    // while (type_temp){
    // }
    
}

// 对 Stmt 的分析
static inline void _Stmt(Node root){
    switch (root->num_child){
    case 2:
        _Exp(root->child[0]);
        break;
    case 3:
        _Exp(root->child[1]);
        break;
    case 4:
        _Exp(root->child[2]);
        break;
    case 7:
        _Exp(root->child[2]);
        break;
    case 5:
        _Exp(root->child[2]);
        break;
    } 
}

// 向外暴露的语义分析器
symbol_node semantic(Node root){
    _initSymbolList();
    _semantic(root);
    // _printSymbolList(head);
    return head;
}

// 语义分析函数
static inline void _semantic(Node root){
    if (root == NULL){
        return;
    }
    // 根据不同的产生式部分进行分析 
    if (!strcmp(root->name, "ExtDef")){
        _ExtDef(root);
    } else if (!strcmp(root->name, "Def")){
        _Def(root);
    } else if (!strcmp(root->name, "Stmt")){
        // 这里用 Stmt 对所有可能的 Exp 进行调用
        // 防止重复调用 Exp 引发的问题
        _Stmt(root); 
    }
    for (int i = 0; i < root->num_child; i++){
        _semantic(root->child[i]);
    }
    // _printSymbolList(head); 
}