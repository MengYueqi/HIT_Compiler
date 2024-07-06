# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <stdlib.h>
# include "semantic.h"
# include "parser.h"

// 设定头指针
symbol_node head = NULL;

// 创建变量表
static inline symbol_node createSymbolNode(){
    // 此函数未实现
}

// 初始化变量表
static inline void _initSymbolList(){
    // 此函数未实现
}

// 打印变量表
static inline void _printSymbolList(symbol_node head){
    // 此函数未实现
}

// 检查变量表中是否有相关数据，有则返回对应指针，没有返回 NULL
static inline symbol_node _findRecord(symbol_node head, symbol_node node){
    // 此函数未实现
}

// 向符号表中添加记录
static inline void _addRecord(symbol_node sym_record){
    // 此函数未实现
}

// 对 ExtDef 的分析
static inline void _ExtDef(Node root){
    // 此函数未实现
}

// 对 CompSt 的分析
static inline type _CompSt(Node root){
    // 此函数未实现
}

// 对 StmtList 的分析
static inline type _StmtList(Node root){
    // 此函数未实现
}

// 对 FunDec 的分析
static inline void _FuncDec(Node root, type return_type){
    // 此函数未实现
}

// 对 VarList 的分析
static inline void _VarList(Node root){
    // 此函数未实现
}

// 对 ParamDec 的分析
static inline void _ParamDec(Node root){
    // 此函数未实现
}

// 对 ExtDecList 进行分析
static inline void _ExtDefList(Node root, type var_type){
    // 此函数未实现
}

// 创建一个 type
static inline type _createType(Kind kind, int num, ...){
    // 此函数未实现
}

// 对 Specifier 的分析
static inline type _Specifier(Node root){
    // 此函数未实现
}

// 对 StructSpecifier 进行分析
static inline void _StructSpecifier(Node root){
    // 此函数未实现
}

// 判断是否有相同的数据
static inline int _hasDuplicateName(symbol_node head){
    // 此函数未实现
}

// 对 DefList 的处理
static inline symbol_node _DefList(Node root){
    // 此函数未实现
}

// 对 OptTag 的处理
static inline void _OptTag(Node root, symbol_node var){
    // 此函数未实现
}

// 对 Tag 的处理
static inline void _Tag(Node root){
    // 此函数未实现
}

// 对 DecList 的分析
static inline void _DecList(Node root, type var_type){
    // 此函数未实现
}

// 对 Dec 的分析
static inline void _Dec(Node root, type var_type){
    // 此函数未实现
}

// 对 VarDec 的分析
static inline void _VarDec(Node root, type var_type){
    // 此函数未实现
}

// 对 Def 的分析
static inline void _Def(Node root){
    // 此函数未实现
}

// 对 Exp 的分析
static inline type _Exp(Node root){
    // 此函数未实现
}

// 对 Args 的分析
static inline void _Args(Node root, symbol_node func_symbol){
    // 此函数未实现
}

// 对 Stmt 的分析
static inline void _Stmt(Node root){
    // 此函数未实现
}

// 向外暴露的语义分析器
void semantic(Node root){
    _initSymbolList();
    _semantic(root);
    // _printSymbolList(head);
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
