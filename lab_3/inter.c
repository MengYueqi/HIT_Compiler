# include "inter.h"
# include <assert.h>
# include <stdarg.h>


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
    if (!strcmp(root->child[1]->name, "DefList")){
        _translateDefList(root->child[1]);
    }
    if (!strcmp(root->child[2]->name, "StmtList")){
        _translateStmtList(root->child[2]);
    }
}

// 对 DefList 进行分析
static inline void _translateDefList(Node root){
    assert(root != NULL);
    while (root->num_child != 0){
        _translateDef(root->child[0]);
        root = root->child[1];
    }
}

// 对 Def 进行分析
static inline void _translateDef(Node root){

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

// InterCode 相关代码
pInterCode newInterCode(int kind, ...) {
    pInterCode p = (pInterCode)malloc(sizeof(InterCode));
    assert(p != NULL);
    p->kind = kind;
    va_list vaList;
    assert(kind >= 0 && kind < 19);
    switch (kind) {
        case IR_LABEL:
        case IR_FUNCTION:
        case IR_GOTO:
        case IR_RETURN:
        case IR_ARG:
        case IR_PARAM:
        case IR_READ:
        case IR_WRITE:
            va_start(vaList, 1);
            p->u.oneOp.op = va_arg(vaList, pOperand);
            break;
        case IR_ASSIGN:
        case IR_GET_ADDR:
        case IR_READ_ADDR:
        case IR_WRITE_ADDR:
        case IR_CALL:
            va_start(vaList, 2);
            p->u.assign.left = va_arg(vaList, pOperand);
            p->u.assign.right = va_arg(vaList, pOperand);
            break;
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
            va_start(vaList, 3);
            p->u.binOp.result = va_arg(vaList, pOperand);
            p->u.binOp.op1 = va_arg(vaList, pOperand);
            p->u.binOp.op2 = va_arg(vaList, pOperand);
            break;
        case IR_DEC:
            va_start(vaList, 2);
            p->u.dec.op = va_arg(vaList, pOperand);
            p->u.dec.size = va_arg(vaList, int);
            break;
        case IR_IF_GOTO:
            va_start(vaList, 4);
            p->u.ifGoto.x = va_arg(vaList, pOperand);
            p->u.ifGoto.relop = va_arg(vaList, pOperand);
            p->u.ifGoto.y = va_arg(vaList, pOperand);
            p->u.ifGoto.z = va_arg(vaList, pOperand);
    }
    return p;
}