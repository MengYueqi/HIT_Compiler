# HIT_Compiler
哈工大2024春编译原理

## 前言
哈工大 2024 年编译原理实验是参考南京大学的编译原理实验，主要为南京大学的前三个实验。在本项目中，简单介绍了我完成这三个实验的一些方法，仅供大家参考。

同时，这个实验比较难以上手，大家可能会在文件结构等方面花费大量时间，所以在这里为大家提供了一种可能的框架，大家可以在这个[框架](./framework/)下完成相关的内容。我希望这个框架能为学弟学妹们节约一些上手的时间，高效快速的的进入代码的核心开发阶段。

*最后，如果你喜欢这个项目的话，欢迎 star ～*

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

- [x]  错误类型1：变量在使用时未经定义。
- [x]  错误类型2：函数在调用时未经定义。
- [x]  错误类型3：变量出现重复定义，或变量与前面定义过的结构体名字重复。
- [x]  错误类型4：函数出现重复定义（即同样的函数名出现了不止一次定义）。
- [x]  错误类型5：赋值号两边的表达式类型不匹配。
- [x]  错误类型6：赋值号左边出现一个只有右值的表达式。
- [x]  错误类型7：操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）。
- [ ]  错误类型8：return语句的返回类型与函数定义的返回类型不匹配。
- [ ]  错误类型9：函数调用时实参与形参的数目或类型不匹配。
- [x]  错误类型10：对非数组型变量使用“[…]”（数组访问）操作符。
- [x]  错误类型11：对普通变量使用“(…)”或“()”（函数调用）操作符。
- [x]  错误类型12：数组访问操作符“[…]”中出现非整数（例如a[1.5]）。
- [x]  错误类型13：对非结构体型变量使用“.”操作符。
- [ ]  错误类型14：访问结构体中未定义过的域。
- [x]  错误类型15：结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化（例如struct A { int a = 0; }）。
- [x]  错误类型16：结构体的名字与前面定义过的结构体或变量的名字重复。
- [ ]  错误类型17：直接使用未定义过的结构体来定义变量。



*PS: 这里由于是参考南京大学的参考书，要求比较多。但实际写到一半我才知道 HIT 只要求完成少部分内容，所以我目前没有写完所有南大的内容，之后有时间会补上的哈哈。*

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

### 语义分析过程
通过查看 `semantic` 可以发现，语义分析是从语法分析树的根节点开始进行的，所以首先需要使用词法和语法分析器构建语法分析树。之后根据这个分析树进行分析。这里分析其实是一个递归分析的过程，具体函数核心代码如下：

```c
// 语义分析函数
static void _semantic(Node root){
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
}
```

可以看到，实际上是从 `_ExtDef`, `_Def`, `_Exp` 开始分析的，在每个节点的函数内，又对语义进行具体的分析，可能调用其他节点。之后算法会继续递归调用其他子节点，进行分析。


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

## 实验3: 中间代码生成
实验 3 的内容为完成一个中间代码生成器。由于课上讲的内容和实验需要完成的内容有较大差别，实验 3 的中间代码生成器为 [NJU 学长](https://github.com/Tyler-ytr/Compiler)的像素级复刻（在这里感谢 NJU Tyler-ytr 学长），仅供大家参考。

大家完成中间代码后，如果需要测试的话，可以使用哈工大学长的[这个项目](https://github.com/ErnestThePoet/ir-virtual-machine?tab=readme-ov-file)。在线上即可完成中间代码的测试，非常方便。

实验 3 的具体代码结构如下：

```
.
├── Makefile
├── inter.c  中间代码生成实现文件
├── inter.h  中间代码生成头文件
├── lex.l
├── lex.yy.c
├── main.c
├── parser
├── parser.h
├── semantic.c
├── semantic.h
├── syntax.tab.c
├── syntax.tab.h
├── syntax.y
├── test
└── tree.c
```



### `inter`文件

在 `inter.c` 文件中，实现了中间代码生成的主要部分。其中，向外暴露了函数接口 `genInter` 在 `main` 文件中通过调用此接口来进行中间代码生成。在 `genInter` 函数中，首先对高级语言进行分析，生成对应的中间代码 List，之后，再调用输出函数，对中间代码进行格式化输出。在 `inter.h` 中声明了存储中间代码的数据结构：

```c
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
```

数据结构中包括了节点的类型和此节点用到的操作符。

### 中间代码生成过程

中间代码生成的过程主要是通过分析语法树递归生成。中间代码生成函数如下：

```c
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
```

可以看到，函数对根节点调用 `_translateExtDefList` 函数进行分析，而在此函数中，又可以调用其他的非终结符节点进行分析。同时 `genInterCodes` 还依次递归调用其子节点，来覆盖所有非终结符节点。

### 编译过程说明

*~~实验 3 建议大家在 Linux 环境下进行编译（Makefile 可以自动识别环境），如果选择在 MacOS 下进行编译，在变量输出的时候可能会出现无法显示的问题。~~ 后来发现 Linux 下也存在问题，我已经将中间变量生成部分的 `sprintf` 函数去除了，解决了这个问题，但问题具体原因仍然不是很清楚。*

在实验 3 中，`makefile` 文件对 Linux 和 MacOS 都进行了适配。`makefile` 文件会自动识别系统并进行编译。如果你使用的是 MacOS 或者 Linux，可以通过执行命令：

```
make
```

进行编译，生成的 `parser` 为编译器的可执行文件。当然，你也可以通过执行以下指令单步进行编译，并查看编译结果：

```
bison -d syntax.y
flex lex.l
gcc -Wall lex.yy.c tree.c syntax.tab.c semantic.c inter.c main.c -o parser -ll -ly
```

这里要注意的是，如果你使用的是 Linux 系统，需要将链接的库从 `-ll` 更改为 `-lfl`。

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

## 参考资料
1. https://github.com/ErnestThePoet/ir-virtual-machine?tab=readme-ov-file
2. https://github.com/Tyler-ytr/Compiler
3. 南京大学编译原理实验指导书