# ifndef INTER
# define INTER
# include "parser.h"
# include "semantic.h"


typedef struct _operand* pOperand;
typedef struct _interCode* pInterCode;
typedef struct _interCodes* pInterCodes;
typedef struct _arg* pArg;
typedef struct _argList* pArgList;
typedef struct _interCodeList* pInterCodeList;

pInterCodeList interCodeList;
symbol_node symTableList;

typedef struct _operand {
    enum {
        OP_VARIABLE,
        OP_CONSTANT,
        OP_ADDRESS,
        OP_LABEL,
        OP_FUNCTION,
        OP_RELOP,
    } kind;

    union {
        int value;
        char* name;
    } u;

    // boolean isAddr;
} Operand;

typedef struct _interCode {
    enum {
        IR_LABEL,
        IR_FUNCTION,
        IR_ASSIGN,
        IR_ADD,
        IR_SUB,
        IR_MUL,
        IR_DIV,
        IR_GET_ADDR,
        IR_READ_ADDR,
        IR_WRITE_ADDR,
        IR_GOTO,
        IR_IF_GOTO,
        IR_RETURN,
        IR_DEC,
        IR_ARG,
        IR_CALL,
        IR_PARAM,
        IR_READ,
        IR_WRITE,
    } kind;

    union {
        struct {
            pOperand op;
        } oneOp;
        struct {
            pOperand right, left;
        } assign;
        struct {
            pOperand result, op1, op2;
        } binOp;
        struct {
            pOperand x, relop, y, z;
        } ifGoto;
        struct {
            pOperand op;
            int size;
        } dec;
    } u;
} InterCode;

typedef struct _interCodes {
    pInterCode code;
    pInterCodes prev, next;
} InterCodes;

typedef struct _arg {
    pOperand op;
    pArg next;
} Arg;

typedef struct _argList {
    pArg head;
    pArg cur;
} ArgList;

typedef struct _interCodeList {
    pInterCodes head;
    pInterCodes cur;
    char* lastArrayName;  // 针对结构体数组，因为需要数组名查表
    int tempVarNum;
    int labelNum;
} InterCodeList;

// 向外暴露接口
void genInter(Node root);

// 所有 Op 相关代码
pOperand newOperand(int kind, ...);
void printOp(FILE* fp, pOperand op);
void setOperand(pOperand p, int kind, void* val);

// 所有 InterCode 相关代码
pInterCode newInterCode(int kind, ...);
void genInterCode(int kind, ...);
void printInterCode(FILE* fp, pInterCodeList interCodeList);
void addInterCode(pInterCodeList interCodeList, pInterCodes newCode);

// InterCodes 相关函数
void genInterCodes(Node root);
pInterCodes newInterCodes(pInterCode code);
pOperand newTemp();
static inline int _getSize(type sym_type);
static inline void _translateExtDefList(Node root);
static inline void _translateExp(Node root, pOperand place);
static inline void _translateCompSt(Node root);
static inline void _translateExtDef(Node root);
static inline void _translateFunDec(Node root);
static inline void _translateDefList(Node root);
static inline void _translateStmtList(Node root);
static inline void _translateStmt(Node root);
static inline void _translateDef(Node root);
static inline void _translateDecList(Node root);
static inline void _translateDec(Node root);
static inline void _translateVarDec(Node root, pOperand place);
static inline void _translateArgs(Node root, pArgList argList);
static inline void _translateCond(Node root, pOperand labelTrue, pOperand labelFalse);
static inline pArgList _newArgList();
static inline pOperand _newLabel();
pInterCodeList newInterCodeList();
static inline char* _newString(char* src);

# endif
