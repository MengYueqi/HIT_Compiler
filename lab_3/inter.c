# include "inter.h"
#include <assert.h>


void genInterCodes(Node root){
    if (root == NULL){
        return;
    } else if (!strcmp(root->name, "ExtDefList")){
        _translateExtDefList(root);
    } else {
        for (int i = 0; i < root->num_child; i++){
            genInterCodes(root->child[i]);
        }
    }
}

// 对 ExtDefList 的翻译
static inline void _translateExtDefList(Node root){
    while (root->num_child != 0){
        // 递归进行分析
        _translateExtDef(root->child[0]);
        root = root->child[1];
    }
}

// 对 ExtDef 的翻译
static inline void _translateExtDef(Node root){
    // 首先判断 root 是否为 NULL
    assert(root != NULL);

    // 对 FunDec 和 CompSt 进行处理
    if (!strcmp(root->child[1]->name, "FunDec")){
        _translateFunDec(root->child[1]);
        _translateCompSt(root->child[2]);
    }
}

// 对 FunDec 进行翻译
static inline void _translateFunDec(Node root){
    // TODO: 核心部分
}

// 对 CompSt 进行翻译
static inline void _translateCompSt(Node root){
    assert(root != NULL);
    if (!strcmp(root->child[1], "DefList")){
        _translateDefList(root->child[1]);
    }
    if (!strcmp(root->child[2], "StmtList")){
        _translateStmtList(root->child[2]);
    }
}

// 对 DefList 进行分析
static inline void _translateDefList(Node root){

}

// 对 StmtList 进行分析
static inline void _translateStmtList(Node root){
    assert(root != NULL);
    for (int i = 0; i < root->num_child; i++){
        if (!strcmp(root->child[i]->name, "Stmt")){
            _translateStmt(root->child[i]);
        }
    }
}

// 对 Stmt 进行分析
static inline void _translateStmt(Node root){
    // TODO: 代码核心部分
}