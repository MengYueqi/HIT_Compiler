# include "inter.h"
# include <assert.h>
# include <stdarg.h>


// 向外暴露接口
void genInter(Node root){
    genInterCodes(root);
    printInterCode(NULL, interCodeList);
}

// 生成中间代码核心代码
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
    assert(root != NULL);
    genInterCode(IR_FUNCTION, newOperand(OP_FUNCTION, _newString(root->child[0]->ID_NAME)));
    if (root->num_child == 4){
        Node temp = root->child[2];
        while(temp->num_child != 1){
            genInterCode(IR_PARAM, newOperand(OP_VARIABLE, _newString(temp->child[1]->ID_NAME)));
            temp = temp->child[2];
        }
    }
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
    assert(root != NULL);
    _translateDecList(root->child[1]);
}

// 对 DecList 进行分析
static inline void _translateDecList(Node root){
    while (root->num_child != 1){
        _translateDec(root->child[0]);
        root = root->child[2];
    }
    if (!strcmp(root->child[0]->name, "Dec")){
        _translateDec(root->child[0]);
    }
    
}

// 对 Dec 进行分析
static inline void _translateDec(Node root){
    assert(root != NULL);
    // 对 Dec -> VarDec 翻译
    if (root->num_child == 1){
        _translateVarDec(root->child[0], NULL);
    }
    // 对 Dec -> VarDec ASSIGNOP Exp 翻译
    else {
        pOperand t1 = newTemp();
        _translateVarDec(root->child[0], t1);
        pOperand t2 = newTemp();
        _translateExp(root->child[2], t2);
        genInterCode(IR_ASSIGN, t1, t2);
    }
}

// 对 VarDec 进行分析
static inline void _translateVarDec(Node root, pOperand place){
    // TODO: 这部分需要改一下，可能要和 lab2 结合
    assert(root != NULL);
    if (!strcmp(root->child[0]->name, "ID")){
        // pItem temp = searchTableItem(table, node->child->val);
        // pType type = temp->field->type;
        interCodeList->tempVarNum--;
        // setOperand(place, OP_VARIABLE, (void*)_newString(root->child[0]->ID_NAME));
        // if (type->kind == BASIC) {
        //     if (place) {
        //         interCodeList->tempVarNum--;
        //         setOperand(place, OP_VARIABLE,
        //                    (void*)newString(temp->field->name));
        //     }
        // } else if (type->kind == ARRAY) {
        //     // 不需要完成高维数组情况
        //     if (type->u.array.elem->kind == ARRAY) {
        //         printf(
        //             "Cannot translate: Code containsvariables of "
        //             "multi-dimensional array type or parameters of array "
        //             "type.\n");
        //         return;
        //     } else {
        //         genInterCode(
        //             IR_DEC,
        //             newOperand(OP_VARIABLE, newString(temp->field->name)),
        //             getSize(type));
        //     }
        // } else if (type->kind == STRUCTURE) {
        //     // 3.1选做
        //     genInterCode(IR_DEC,
        //                  newOperand(OP_VARIABLE, newString(temp->field->name)),
        //                  getSize(type));
        // }
    } else {
        _translateVarDec(root->child[0], place);
    } 
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
    assert(root != NULL);
    // 对不同情况进行解析
    if (!strcmp(root->child[0]->name, "Exp")){
        _translateExp(root->child[0], NULL);
    } else if (!strcmp(root->child[0]->name, "CompSt")){
        _translateCompSt(root->child[0]);
    }
}

// 对 Exp 进行分析
static inline void _translateExp(Node root, pOperand place){
    // TODO: 代码核心部分
}

// Op 相关代码
// 打印 Op 代码
void printOp(FILE* fp, pOperand op) {
    assert(op != NULL);
    if (fp == NULL) {
        switch (op->kind) {
            case OP_CONSTANT:
                printf("#%d", op->u.value);
                break;
            case OP_VARIABLE:
            case OP_ADDRESS:
            case OP_LABEL:
            case OP_FUNCTION:
            case OP_RELOP:
                printf("%s", op->u.name);
                break;
        }
    } else {
        switch (op->kind) {
            case OP_CONSTANT:
                fprintf(fp, "#%d", op->u.value);
                break;
            case OP_VARIABLE:
            case OP_ADDRESS:
            case OP_LABEL:
            case OP_FUNCTION:
            case OP_RELOP:
                fprintf(fp, "%s", op->u.name);
                break;
        }
    }
}

// 创建一个 Op
pOperand newOperand(int kind, ...){
    pOperand p = (pOperand)malloc(sizeof(Operand));
    assert(p != NULL);
    p->kind = kind;
    va_list vaList;
    assert(kind >= 0 && kind < 6);
    va_start(vaList, 1);
    switch (kind) {
        case OP_CONSTANT:
            p->u.value = va_arg(vaList, int);
            break;
        case OP_VARIABLE:
        case OP_ADDRESS:
        case OP_LABEL:
        case OP_FUNCTION:
        case OP_RELOP:
            p->u.name = va_arg(vaList, char*);
            break;
    }
    // p->isAddr = FALSE;
    return p;
}

// InterCode 相关代码
// 生成一个 InterCode
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

// 打印一个 InterCode
void printInterCode(FILE* fp, pInterCodeList interCodeList){
    for (pInterCodes cur = interCodeList->head; cur != NULL; cur = cur->next) {
        assert(cur->code->kind >= 0 && cur->code->kind < 19);
        if (fp == NULL) {
            switch (cur->code->kind) {
                case IR_LABEL:
                    printf("LABEL ");
                    printOp(fp, cur->code->u.oneOp.op);
                    printf(" :");
                    break;
                case IR_FUNCTION:
                    printf("FUNCTION ");
                    printOp(fp, cur->code->u.oneOp.op);
                    printf(" :");
                    break;
                case IR_ASSIGN:
                    printOp(fp, cur->code->u.assign.left);
                    printf(" := ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_ADD:
                    printOp(fp, cur->code->u.binOp.result);
                    printf(" := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    printf(" + ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_SUB:
                    printOp(fp, cur->code->u.binOp.result);
                    printf(" := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    printf(" - ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_MUL:
                    printOp(fp, cur->code->u.binOp.result);
                    printf(" := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    printf(" * ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_DIV:
                    printOp(fp, cur->code->u.binOp.result);
                    printf(" := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    printf(" / ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_GET_ADDR:
                    printOp(fp, cur->code->u.assign.left);
                    printf(" := &");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_READ_ADDR:
                    printOp(fp, cur->code->u.assign.left);
                    printf(" := *");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_WRITE_ADDR:
                    printf("*");
                    printOp(fp, cur->code->u.assign.left);
                    printf(" := ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_GOTO:
                    printf("GOTO ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_IF_GOTO:
                    printf("IF ");
                    printOp(fp, cur->code->u.ifGoto.x);
                    printf(" ");
                    printOp(fp, cur->code->u.ifGoto.relop);
                    printf(" ");
                    printOp(fp, cur->code->u.ifGoto.y);
                    printf(" GOTO ");
                    printOp(fp, cur->code->u.ifGoto.z);
                    break;
                case IR_RETURN:
                    printf("RETURN ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_DEC:
                    printf("DEC ");
                    printOp(fp, cur->code->u.dec.op);
                    printf(" ");
                    printf("%d", cur->code->u.dec.size);
                    break;
                case IR_ARG:
                    printf("ARG ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_CALL:
                    printOp(fp, cur->code->u.assign.left);
                    printf(" := CALL ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_PARAM:
                    printf("PARAM ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_READ:
                    printf("READ ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_WRITE:
                    printf("WRITE ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
            }
            printf("\n");
        } else {
            switch (cur->code->kind) {
                case IR_LABEL:
                    fprintf(fp, "LABEL ");
                    printOp(fp, cur->code->u.oneOp.op);
                    fprintf(fp, " :");
                    break;
                case IR_FUNCTION:
                    fprintf(fp, "FUNCTION ");
                    printOp(fp, cur->code->u.oneOp.op);
                    fprintf(fp, " :");
                    break;
                case IR_ASSIGN:
                    printOp(fp, cur->code->u.assign.left);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_ADD:
                    printOp(fp, cur->code->u.binOp.result);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    fprintf(fp, " + ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_SUB:
                    printOp(fp, cur->code->u.binOp.result);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    fprintf(fp, " - ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_MUL:
                    printOp(fp, cur->code->u.binOp.result);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    fprintf(fp, " * ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_DIV:
                    printOp(fp, cur->code->u.binOp.result);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.binOp.op1);
                    fprintf(fp, " / ");
                    printOp(fp, cur->code->u.binOp.op2);
                    break;
                case IR_GET_ADDR:
                    printOp(fp, cur->code->u.assign.left);
                    fprintf(fp, " := &");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_READ_ADDR:
                    printOp(fp, cur->code->u.assign.left);
                    fprintf(fp, " := *");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_WRITE_ADDR:
                    fprintf(fp, "*");
                    printOp(fp, cur->code->u.assign.left);
                    fprintf(fp, " := ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_GOTO:
                    fprintf(fp, "GOTO ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_IF_GOTO:
                    fprintf(fp, "IF ");
                    printOp(fp, cur->code->u.ifGoto.x);
                    fprintf(fp, " ");
                    printOp(fp, cur->code->u.ifGoto.relop);
                    fprintf(fp, " ");
                    printOp(fp, cur->code->u.ifGoto.y);
                    fprintf(fp, " GOTO ");
                    printOp(fp, cur->code->u.ifGoto.z);
                    break;
                case IR_RETURN:
                    fprintf(fp, "RETURN ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_DEC:
                    fprintf(fp, "DEC ");
                    printOp(fp, cur->code->u.dec.op);
                    fprintf(fp, " ");
                    fprintf(fp, "%d", cur->code->u.dec.size);
                    break;
                case IR_ARG:
                    fprintf(fp, "ARG ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_CALL:
                    printOp(fp, cur->code->u.assign.left);
                    fprintf(fp, " := CALL ");
                    printOp(fp, cur->code->u.assign.right);
                    break;
                case IR_PARAM:
                    fprintf(fp, "PARAM ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_READ:
                    fprintf(fp, "READ ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
                case IR_WRITE:
                    fprintf(fp, "WRITE ");
                    printOp(fp, cur->code->u.oneOp.op);
                    break;
            }
            fprintf(fp, "\n");
        }
    }
}

// 向 CodeList 中添加一个 InterCode 节点
void genInterCode(int kind, ...){
    va_list vaList;
    pOperand temp = NULL;
    pOperand result = NULL, op1 = NULL, op2 = NULL, relop = NULL;
    int size = 0;
    pInterCodes newCode = NULL;
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
            op1 = va_arg(vaList, pOperand);
            if (op1->kind == OP_ADDRESS) {
                temp = newTemp();
                genInterCode(IR_READ_ADDR, temp, op1);
                op1 = temp;
            }
            newCode = newInterCodes(newInterCode(kind, op1));
            addInterCode(interCodeList, newCode);
            break;
        case IR_ASSIGN:
        case IR_GET_ADDR:
        case IR_READ_ADDR:
        case IR_WRITE_ADDR:
        case IR_CALL:
            va_start(vaList, 2);
            op1 = va_arg(vaList, pOperand);
            op2 = va_arg(vaList, pOperand);
            if (kind == IR_ASSIGN &&
                (op1->kind == OP_ADDRESS || op2->kind == OP_ADDRESS)) {
                if (op1->kind == OP_ADDRESS && op2->kind != OP_ADDRESS)
                    genInterCode(IR_WRITE_ADDR, op1, op2);
                else if (op2->kind == OP_ADDRESS && op1->kind != OP_ADDRESS)
                    genInterCode(IR_READ_ADDR, op1, op2);
                else {
                    temp = newTemp();
                    genInterCode(IR_READ_ADDR, temp, op2);
                    genInterCode(IR_WRITE_ADDR, op1, temp);
                }
            } else {
                newCode = newInterCodes(newInterCode(kind, op1, op2));
                addInterCode(interCodeList, newCode);
            }
            break;
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
            va_start(vaList, 3);
            result = va_arg(vaList, pOperand);
            op1 = va_arg(vaList, pOperand);
            op2 = va_arg(vaList, pOperand);
            if (op1->kind == OP_ADDRESS) {
                temp = newTemp();
                genInterCode(IR_READ_ADDR, temp, op1);
                op1 = temp;
            }
            if (op2->kind == OP_ADDRESS) {
                temp = newTemp();
                genInterCode(IR_READ_ADDR, temp, op2);
                op2 = temp;
            }
            newCode = newInterCodes(newInterCode(kind, result, op1, op2));
            addInterCode(interCodeList, newCode);
            break;
        case IR_DEC:
            // TODO:
            va_start(vaList, 2);
            op1 = va_arg(vaList, pOperand);
            size = va_arg(vaList, int);
            newCode = newInterCodes(newInterCode(kind, op1, size));
            addInterCode(interCodeList, newCode);
            break;
        case IR_IF_GOTO:
            // TODO:
            va_start(vaList, 4);
            result = va_arg(vaList, pOperand);
            relop = va_arg(vaList, pOperand);
            op1 = va_arg(vaList, pOperand);
            op2 = va_arg(vaList, pOperand);
            newCode = newInterCodes(newInterCode(kind, result, relop, op1, op2));
            addInterCode(interCodeList, newCode);
            break;
    }
}

// 添加一个 InterCode 到 List
void addInterCode(pInterCodeList interCodeList, pInterCodes newCode){
    if (interCodeList->head == NULL) {
        interCodeList->head = newCode;
        interCodeList->cur = newCode;
    } else {
        interCodeList->cur->next = newCode;
        newCode->prev = interCodeList->cur;
        interCodeList->cur = newCode;
    }
}

// 中间类型的函数
pOperand newTemp(){
    // printf("newTemp() tempVal:%d\n", interCodeList->tempVarNum);
    char tName[10] = {0};
    sprintf(tName, "t%d", interCodeList->tempVarNum);
    interCodeList->tempVarNum++;
    pOperand temp = newOperand(OP_VARIABLE, tName);
    return temp;
}

// InterCodes 相关函数
pInterCodes newInterCodes(pInterCode code) {
    pInterCodes p = (pInterCodes)malloc(sizeof(InterCodes));
    assert(p != NULL);
    p->code = code;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

// 创建一个 CodeList
pInterCodeList newInterCodeList(){
    pInterCodeList p = (pInterCodeList)malloc(sizeof(InterCodeList));
    p->head = NULL;
    p->cur = NULL;
    p->lastArrayName = NULL;
    p->tempVarNum = 1;
    p->labelNum = 1;
    return p;
}

// String 的中间函数
static inline char* _newString(char* src){
    if (src == NULL) return NULL;
    int length = strlen(src) + 1;
    char* p = (char*)malloc(sizeof(char) * length);
    assert(p != NULL);
    strncpy(p, src, length);
    return p;
}