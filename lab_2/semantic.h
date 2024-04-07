#ifndef __SEMANTIC_H
#define __SEMANTIC_H

# include "parser.h"

void semantic(Node root);
typedef struct _SymbolNode *symbol_node;
typedef enum {BASIC, ARRAY, STRUCTURE} Kind;  // 三种类型数据
typedef enum {INT, FLOAT} basic_type;  // 基本类型
typedef struct _Type *type;  // 类型指针

// 声明一个数据的类型
struct _Type{
    Kind kind;
    union{
        // 基本的数据类型
        basic_type basic;
        // 数组类型
        struct {type arr_type; int size;} arr;
        // 结构体类型
        symbol_node struct_pointer;
    }data;
};

// 变量单元指针
struct _SymbolNode{
    char *name;
    type symbolType;
    symbol_node next;
};

// 内部函数的声明
static void _initSymbolList();
static void _printSymbolList(symbol_node head);
static int _checkRecord(symbol_node node);
static void _addRecord(symbol_node sym_record);
static void _ExtDef(Node root);
static type _createType(Kind kind, int num, ...);
static type _Specifier(Node root);
static symbol_node _createSymbolNode();
static void _DecList(Node root, type var_type);
static void _Dec(Node root, type var_type);
static void _VarDec(Node root, type var_type);
static void _semantic(Node root);
static void _Def(Node root);
static void _Exp(Node root);

#endif