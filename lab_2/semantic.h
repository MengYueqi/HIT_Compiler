#ifndef __SEMANTIC_H
#define __SEMANTIC_H

# include "parser.h"

void semantic(Node root);
typedef struct _SymbolList *symbol_list;
typedef enum {BASIC, ARRAY, STRUCTURE} Kind;  // 三种类型数据
typedef enum {INT, FLOAT} basic_type;  // 基本类型
typedef struct _Type *type;  // 类型指针

// 声明一个数据的类型
struct _Type{
    Kind kind;
    union data{
        // 基本的数据类型
        basic_type basic;
        // 数组类型
        struct {type arr_type; int size;} arr;
        // 结构体类型
        symbol_list struct_pointer;
    };
};

// 变量单元指针
struct _SymbolList{
    char *name;
    type type;
    symbol_list next;
};


#endif