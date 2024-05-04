# include "inter.h"
# include "semantic.h"
# include <assert.h>
# include <stdarg.h>

int count = 0;

// 向外暴露接口
void genInter(Node root){
    genInterCodes(root);
    FILE* fw = fopen("./log.out", "wt+");
    if (!fw) {
        return;
    }
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
    assert(root != NULL);
    if (!strcmp(root->child[0]->name, "ID")){
        symbol_node tar = createSymbolNode();
        tar->name = root->child[0]->ID_NAME;
        symbol_node temp = findRecord(symTableList, tar);
        type sym_type = temp->symbolType;
        if (sym_type->kind == BASIC) {
            if (place) {
                interCodeList->tempVarNum--;
                setOperand(place, OP_VARIABLE,
                           (void*)_newString(root->child[0]->ID_NAME));
            }
        } else if (sym_type->kind == ARRAY) {
                genInterCode(
                    IR_DEC,
                    newOperand(OP_VARIABLE, _newString(root->child[0]->ID_NAME)),
                    _getSize(sym_type->data.arr.size));
        }
    } else {
        _translateVarDec(root->child[0], place);
    } 
}

// 对 StmtList 进行分析
static inline void _translateStmtList(Node root){
    assert(root != NULL);
    while (root->num_child == 2){
        if (!strcmp(root->child[0]->name, "Stmt")){
            _translateStmt(root->child[0]);
            root = root->child[1];
        }
    }
}

// 对 Stmt 进行分析
static inline void _translateStmt(Node root){
    assert(root != NULL);
    // 对不同情况进行解析

    // Exp SEMI
    if (!strcmp(root->child[0]->name, "Exp")){
        _translateExp(root->child[0], NULL);
    } 
    
    // CompSt
    else if (!strcmp(root->child[0]->name, "CompSt")){
        _translateCompSt(root->child[0]);
    } 
    
    // RETURN Exp SEMI
    else if (!strcmp(root->child[0]->name, "RETURN")) {
        pOperand t1 = newTemp();
        _translateExp(root->child[1], t1);
        genInterCode(IR_RETURN, t1);
    }

    // IF LP Exp RP Stmt
    else if (!strcmp(root->child[0]->name, "IF")) {
        Node exp = root->child[2];
        Node stmt = root->child[4];
        pOperand label1 = _newLabel();
        pOperand label2 = _newLabel();

        _translateCond(exp, label1, label2);
        genInterCode(IR_LABEL, label1);
        _translateStmt(stmt);
        if (root->num_child == 5) {
            genInterCode(IR_LABEL, label2);
        }
        // IF LP Exp RP Stmt ELSE Stmt
        else {
            pOperand label3 = _newLabel();
            genInterCode(IR_GOTO, label3);
            genInterCode(IR_LABEL, label2);
            _translateStmt(root->child[6]);
            genInterCode(IR_LABEL, label3);
        }

    }

    // WHILE LP Exp RP Stmt
    else if (!strcmp(root->child[0]->name, "WHILE")) {
        pOperand label1 = _newLabel();
        pOperand label2 = _newLabel();
        pOperand label3 = _newLabel();

        genInterCode(IR_LABEL, label1);
        _translateCond(root->child[2], label2, label3);
        genInterCode(IR_LABEL, label2);
        _translateStmt(root->child[4]);
        genInterCode(IR_GOTO, label1);
        genInterCode(IR_LABEL, label3);
    }
}

// 对 Exp 进行分析
static inline void _translateExp(Node root, pOperand place){

    assert(root != NULL);

    // Exp -> LP Exp RP
    if (!strcmp(root->child[0]->name, "LP")){
        _translateExp(root->child[1], place);
    }

    else if (!strcmp(root->child[0]->name, "Exp") || !strcmp(root->child[0]->name, "NOT")) {
        // 条件表达式 和 基本表达式
        if (strcmp(root->child[1]->name, "LB") && strcmp(root->child[1]->name, "DOT")) {
            // Exp -> Exp AND Exp
            //      | Exp OR Exp
            //      | Exp RELOP Exp
            //      | NOT Exp
            if (!strcmp(root->child[1]->name, "AND") || !strcmp(root->child[1]->name, "OR") ||
                !strcmp(root->child[1]->name, "RELOP") || !strcmp(root->child[0]->name, "NOT")) {
                pOperand label1 = _newLabel();
                pOperand label2 = _newLabel();
                pOperand true_num = newOperand(OP_CONSTANT, 1);
                pOperand false_num = newOperand(OP_CONSTANT, 0);
                genInterCode(IR_ASSIGN, place, false_num);
                _translateCond(root, label1, label2);
                genInterCode(IR_LABEL, label1);
                genInterCode(IR_ASSIGN, place, true_num);
            } else {
                // Exp -> Exp ASSIGNOP Exp
                if (!strcmp(root->child[1]->name, "ASSIGNOP")) {
                    pOperand t1 = newTemp();
                    _translateExp(root->child[0], t1);
                    pOperand t2 = newTemp();
                    _translateExp(root->child[2], t2);
                    printf("ASS: %s\n\n", t2->u.name);
                    genInterCode(IR_ASSIGN, t1, t2);
                } else {
                    pOperand t1 = newTemp();
                    _translateExp(root->child[0], t1);
                    pOperand t2 = newTemp();
                    _translateExp(root->child[2], t2);
                    // Exp -> Exp PLUS Exp
                    if (!strcmp(root->child[1]->name, "PLUS")) {
                        genInterCode(IR_ADD, place, t1, t2);
                    }
                    // Exp -> Exp MINUS Exp
                    else if (!strcmp(root->child[1]->name, "MINUS")) {
                        genInterCode(IR_SUB, place, t1, t2);
                    }
                    // Exp -> Exp STAR Exp
                    else if (!strcmp(root->child[1]->name, "STAR")) {
                        genInterCode(IR_MUL, place, t1, t2);
                    }
                    // Exp -> Exp DIV Exp
                    else if (!strcmp(root->child[1]->name, "DIV")) {
                        genInterCode(IR_DIV, place, t1, t2);
                    }
                }
            }

        }
        // 数组和结构体访问
        else {
            // Exp -> Exp LB Exp RB
            if (!strcmp(root->child[1]->name, "LB")) {
                //数组
                pOperand idx = newTemp();
                _translateExp(root->child[2], idx);
                pOperand base = newTemp();
                _translateExp(root->child[0], base);
                pOperand width;
                pOperand offset = newTemp();
                pOperand target;
                // 根据假设，Exp1只会展开为 Exp DOT ID 或 ID
                // 我们让前一种情况吧ID作为name回填进place返回到这里的base处，在语义分析时将结构体变量也填进表（因为假设无重名），这样两种情况都可以查表得到。
                symbol_node tar = createSymbolNode();
                tar->name = root->child[0]->ID_NAME;
                symbol_node item = findRecord(symTableList, tar);
                assert(item->symbolType->kind == ARRAY);
                width = newOperand(
                    OP_CONSTANT, _getSize(item->symbolType->data.arr.arr_type));
                genInterCode(IR_MUL, offset, idx, width);
                // 如果是ID[Exp],
                // 则需要对ID取址，如果前面是结构体内访问，则会返回一个地址类型，不需要再取址
                if (base->kind == OP_VARIABLE) {
                // printf("非结构体数组访问\n");
                target = newTemp();
                genInterCode(IR_GET_ADDR, target, base);
                } else {
                // printf("结构体数组访问\n");
                    target = base;
                }
                genInterCode(IR_ADD, place, target, offset);
                place->kind = OP_ADDRESS;
                interCodeList->lastArrayName = base->u.name;
            }
            // Exp -> Exp DOT ID
            else {
                //结构体
                pOperand temp = newTemp();
                _translateExp(root->child[0], temp);
                // 两种情况，Exp直接为一个变量，则需要先取址，若Exp为数组或者多层结构体访问或结构体形参，则target会被填成地址，可以直接用。
                pOperand target;

                if (temp->kind == OP_ADDRESS) {
                    target = newOperand(temp->kind, temp->u.name);
                    // target->isAddr = TRUE;
                } else {
                    target = newTemp();
                    genInterCode(IR_GET_ADDR, target, temp);
                }

                pOperand id = newOperand(
                    OP_VARIABLE, _newString(root->child[2]->ID_NAME));
                int offset = 0;
                symbol_node tar = createSymbolNode();
                tar->name = root->child[0]->ID_NAME;
                symbol_node item = findRecord(symTableList, tar);
                // //结构体数组，temp是临时变量，查不到表，需要用处理数组时候记录下的数组名老查表
                // if (item == NULL) {
                //     item = searchTableItem(table, interCodeList->lastArrayName);
                // }

                // pFieldList tmp;
                // // 结构体数组 eg: a[5].b
                // if (item->field->type->kind == ARRAY) {
                //     tmp = item->field->type->u.array.elem->u.structure.field;
                // }
                // // 一般结构体
                // else {
                //     tmp = item->field->type->u.structure.field;
                // }
                // // 遍历获得offset
                // while (tmp) {
                //     if (!strcmp(tmp->name, id->u.name)) break;
                //     offset += getSize(tmp->type);
                //     tmp = tmp->tail;
                // }
                // pOperand tOffset = newOperand(OP_CONSTANT, offset);
                // if (place) {
                //     genInterCode(IR_ADD, place, target, tOffset);
                //     // 为了处理结构体里的数组把id名通过place回传给上层
                //     setOperand(place, OP_ADDRESS, (void*)newString(id->u.name));
                //     // place->isAddr = TRUE;
                // }
            }
        }
    }
    //单目运算符
    // Exp -> MINUS Exp
    else if (!strcmp(root->child[0]->name, "MINUS")) {
        pOperand t1 = newTemp();
        _translateExp(root->child[1], t1);
        pOperand zero = newOperand(OP_CONSTANT, 0);
        genInterCode(IR_SUB, place, zero, t1);
    } else if (!strcmp(root->child[0]->name, "ID") && root->child[1]) {
        pOperand funcTemp =
            newOperand(OP_FUNCTION, _newString(root->child[0]->ID_NAME));
        // Exp -> ID LP Args RP
        if (!strcmp(root->child[2]->name, "Args")) {
            pArgList argList = _newArgList();
            _translateArgs(root->child[2], argList);
            if (!strcmp(root->child[0]->name, "write")) {
                genInterCode(IR_WRITE, argList->head->op);
            } else {
                pArg argTemp = argList->head;
                while (argTemp) {
                    if (argTemp->op == OP_VARIABLE) {
                        symbol_node tar = createSymbolNode();
                        tar->name = root->child[0]->ID_NAME;
                        symbol_node item = findRecord(symTableList, tar);

                        // 结构体作为参数需要传址
                        if (item && item->symbolType->kind == STRUCTURE) {
                            pOperand varTemp = newTemp();
                            genInterCode(IR_GET_ADDR, varTemp, argTemp->op);
                            pOperand varTempCopy =
                                newOperand(OP_ADDRESS, varTemp->u.name);
                            genInterCode(IR_ARG, varTempCopy);
                        }
                    }
                    // 一般参数直接传值
                    else {
                        genInterCode(IR_ARG, argTemp->op);
                    }
                    argTemp = argTemp->next;
                }
                if (place) {
                    genInterCode(IR_CALL, place, funcTemp);
                } else {
                    pOperand temp = newTemp();
                    genInterCode(IR_CALL, temp, funcTemp);
                }
            }
        }
        // Exp -> ID LP RP
        else {
            if (!strcmp(root->child[0]->ID_NAME, "read")) {
                genInterCode(IR_READ, place);
            } else {
                if (place) {
                    genInterCode(IR_CALL, place, funcTemp);
                } else {
                    pOperand temp = newTemp();
                    genInterCode(IR_CALL, temp, funcTemp);
                }
            }
        }
    }
    // Exp -> ID
    else if (!strcmp(root->child[0]->name, "ID")) {
        symbol_node tar = createSymbolNode();
        tar->name = root->child[0]->ID_NAME;
        symbol_node item = findRecord(symTableList, tar);
        // 根据讲义，因为结构体不允许赋值，结构体做形参时是传址的方式
        interCodeList->tempVarNum--;
        if (item->symbolType->kind == STRUCTURE) {
            setOperand(place, OP_ADDRESS, (void*)_newString(root->child[0]->ID_NAME));
        }
        // 非结构体参数情况都当做变量处理
        else {
            setOperand(place, OP_VARIABLE, (void*)_newString(root->child[0]->ID_NAME));
        }
    } else {
        // TODO: 这里本身是处理 INT 和 FLOAT，但是 Exp 不能直接通过调用自己来处理 INT
        interCodeList->tempVarNum--;
        setOperand(place, OP_CONSTANT, (void*)atoi(root->child[0]->ID_NAME));
    }
}

// 对 Cond 的分析
static inline void _translateCond(Node root, pOperand labelTrue, pOperand labelFalse){

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
            // printf("Here: %s\n", p->u.name);
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
            va_start(vaList, 2);
            op1 = va_arg(vaList, pOperand);
            size = va_arg(vaList, int);
            newCode = newInterCodes(newInterCode(kind, op1, size));
            addInterCode(interCodeList, newCode);
            break;
        case IR_IF_GOTO:
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
    // interCodeList->tempVarNum++;
    char name[10] = "";
    sprintf(name, "t%d", count++);
    // printf("%s\n", name);
    pOperand temp = newOperand(OP_VARIABLE, name);
    temp->u.name = name;
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

// 获取变量所占空间大小
static inline int _getSize(type sym_type) {
    if (sym_type == NULL){
        return 0;
    }
    else if (sym_type->kind == BASIC){
        return 4;
    }
    else if (sym_type->kind == ARRAY){
        return sym_type->data.arr.size * _getSize(sym_type->data.arr.arr_type);
    }
    return 0;
}

void setOperand(pOperand p, int kind, void* val){
    assert(p != NULL);
    assert(kind >= 0 && kind < 6);
    p->kind = kind;
    switch (kind) {
        case OP_CONSTANT:
            p->u.value = (int)val;
            break;
        case OP_VARIABLE:
        case OP_ADDRESS:
        case OP_LABEL:
        case OP_FUNCTION:
        case OP_RELOP:
            // if (p->u.name) free(p->u.name);
            p->u.name = (char*)val;
            break;
    }
}

// 对 Args 的分析
static inline void _translateArgs(Node root, pArgList argList){
    // TODO: 核心部分
}

// 创建一个 ArgList
static inline pArgList _newArgList(){
    pArgList p = (pArgList)malloc(sizeof(ArgList));
    assert(p != NULL);
    p->head = NULL;
    p->cur = NULL;
}

// 创建一个新的 label
static inline pOperand _newLabel(){
    char lName[10] = {0};
    sprintf(lName, "label%d", interCodeList->labelNum);
    interCodeList->labelNum++;
    pOperand temp = newOperand(OP_LABEL, _newString(lName));
    return temp;
}