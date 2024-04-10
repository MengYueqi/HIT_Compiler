# HIT_Compiler
哈工大2024春编译原理

## 实验1: 词法分析与语法分析
实验 1 为使用 GNU Flex 和 GNU Bison 完成词法分析和语法分析。其中文法定义参考[文法定义](./syntax_definition.md)。这是一个非常非常老的编译器编写方式，而且不是很好上手。以下对代码进行简单介绍，希望能帮学弟学妹们快速上手。

首先，代码的架构如下：
```
.
├── Makefile
├── lex.l   词法分析器
├── lex.yy.c    词法分析器编译后的文件
├── main.c  主函数
├── parser  编译生成的可执行文件
├── parser.h    头文件
├── syntax.tab.c    语法分析器编译生成的.c文件
├── syntax.tab.h    语法分析器编译生成的.h文件
├── syntax.y    语法分析器
└── tree.c  语法树实现相关代码
```

### Flex 文件
我们首先需要编写词法分析器，具体的语法大家 GPT 一下就明白。词法分析器在 `lex.l` 中进行编写。编写完成后，在 MacOS 或 Linux 下使用命令：
```
flex lex.l
```
完成编译，编译生成 `lex.yy.c` 文件。此文件为词法分析器对应的 C 语言文件。使用 GCC 对文件进行编译，之后执行文件，如下：
```
gcc lex.yy.c -o test
./test
```
即可完成词法分析。但要注意，不能直接使用本项目中的 `lex.l` 文件进行以上操作实现词法分析。因为为了完成实验要求，本项目中的 `lex.l` 还引用了了其他文件且没有 `main` 函数。

### Bison 文件
语法分析器主要是在 `syntax.y` 中进行的实现。这里的语法也比较固定。其中几个比较重要的是：首先，要在首部生命一个 `union`，用来在语法分析器和词法分析器中传递数据。其次，要注意终结符要用 `token` 声明这些符号，非终结符字符用 `type` 声明。终结符和非终结符都是 `Node` 类型。完成 `syntax.y` 后使用如下命令进行编译：
```
bison -d syntax.y
```
会生成 `syntax.tab.c` `syntax.tab.h` 两个文件。

### 语法树实现
在完成语法分析器和词法分析器后，我们还需要完成语法树的实现。这里实现一个树节点 `Node` 并且在语法分析和词法分析中，创建对应的节点。具体创建的部分的实现在对应产生式的花括号中。这里：`$n` 表示规则匹配到的第 n 个子节点的语义值。`$$` 表示生成式左部的语义值。而具体语法分析树的实现过程在文件 `tree.c` 中，核心思想是对语法树进行一个前序遍历。

### 编译过程说明
如需使用该项目的代码内容，可以在 MacOS 下使用命令进行编译：
```
make
```
如果是 Linux 环境，需要注意，在最后编译生成可执行文件 `parser` 时，需要将链接库的选项由 `-ll` 改为 `-lfl`。具体编译的顺序如下：
```
bison -d syntax.y
flex lex.l
gcc -Wall lex.yy.c tree.c syntax.tab.c main.c -o parser -ll
```
你也可以逐步进行编译，并查看编译的结果。

## 实验2: 语义分析
实验 2 为在之前的基础上，实现一个语义分析器。与之前不同的是，实验二全部需要手写而不是借助其他工具完成。实验二具体要求在[此文件](./semantic_definition.md)中给出。

目前已经完成的进度：
![graph](./lab_2/graph/funcs.png)

实验 2 的具体结构如下：
```
.
├── Makefile
├── lex.l
├── lex.yy.c
├── main.c
├── parser
├── parser.h
├── semantic.c  语义分析文件
├── semantic.h  语义分析头文件
├── syntax.tab.c
├── syntax.tab.h
├── syntax.y
├── test
└── tree.c
```

### `semantic` 文件
在 `semantic` 文件中，是对语法树进行语义分析。这里所有的内部调用的函数都是以 `_` 开头，并且为 `static` 函数，避免外部进行调用。同时，在 `semantic` 文件中仅向外面暴露一个 `semantic` 方法用于语义分析。在 `semantic` 文件中，对每个需要分析的非终结符都有对应的分析方法。在 `_semantic` 方法中，采用递归分析对输入的节点及他的所有儿子进行分析。

分析过程中，需要建立变量表，具体的变量数据结构为链表，节点为 `semantic.h` 中的 `_SymbolNode`。对于每一个变量，维护一个节点。在 `semantic.h` 中给出了类型和变量单元的定义，变量单元定义如下：

```c
// 变量单元
struct _SymbolNode{
    char *name;
    type symbolType;
    symbol_node next;
};
```

其中每个节点包含节点名，节点类型和下一个节点的指针。类型的定义如下：

```c
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
        // 函数类型，包含返回类型喝参数类型
        struct{type return_type; symbol_node par_type}func_type;
    }data;
};
```

其中 `kind` 为基本类型，`data` 为基本类型的必要补充。另外，在 `semantic.h` 中还有其他函数的声明，在 `semantic.c` 中包含对具体的函数的实现。

### 编译过程说明
如需使用该项目的代码内容，可以在 MacOS 下使用命令进行编译：
```
make
```
如果是 Linux 环境，需要注意，在最后编译生成可执行文件 `parser` 时，需要将链接库的选项由 `-ll` 改为 `-lfl`。具体编译的顺序如下：
```
bison -d syntax.y
flex lex.l 
gcc -Wall lex.yy.c tree.c syntax.tab.c semantic.c main.c -o parser -ll
```
你也可以逐步进行编译，并查看编译的结果。

## 一些踩雷的点
在写这个实验的时候我有很长时间没碰过 c 语言了，所以非常不熟悉。在这里写一些过程中踩到的雷，大佬请忽略。
1. c 语言的头文件一定要加上

```c
#ifndef __PARSER_H
#define __PARSER_H
#endif
```
尤其是用 VScode 编程的时候，因为这个要用的头文件比较多，一旦没加的话，搞不好就重定义了，非常的麻烦。

2. 要注意一下实验二里变量表的节点类型为 `_SymbolNode`，而树的节点类型为 `Node`，要区分一下，不然容易段错误，非常难处理。

## 说明
1. 此项目仅供学习交流，请勿将此项目的代码用作商业用途。如用作商业用途，作者不承担任何责任。
2. 如果你需要任何课程相关的其他资料，请联系我的邮箱：mengyq2002@gmail.com。
2. 感谢 Open AI [ChatGPT](https://chat.openai.com) 和 Google [Gemini](https://gemini.google.com/app/) 的大力支持。