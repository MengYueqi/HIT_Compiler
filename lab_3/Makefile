# Makefile for compiling Flex-generated lexer and Bison-generated parser

# 编译器
CC = gcc

# Flex编译器
FLEX = flex

# Bison编译器
BISON = bison

# 编译器标志
CFLAGS = -Wall -O2

# 目标可执行文件
TARGET = parser

# Bison源文件
BISON_FLAGS = -d

# 源文件
LEX_SOURCE = lex.l
LEX_C = lex.yy.c
MAIN = main.c
TREE_SOURCE = tree.c
BISON_SOURCE = syntax.y
BISON_OUT = syntax.tab.c
SEMANTIC = semantic.c
INTER = inter.c

# 获取系统信息
ARCH := $(shell uname -s)
MACOS = Darwin
LINUX = Linux

# 针对不同平台的变量设置
ifeq ($(ARCH), $(MACOS))
	LIBS = -ll -ly
else ifeq ($(ARCH), $(LINUX))
	LIBS = -lfl
endif

all: build

build: $(TARGET)

$(TARGET): $(LEX_C) $(SEMANTIC) $(BISON_OUT) $(MAIN) $(TREE_SOURCE) $(INTER)
	$(CC) $(CFLAGS) $(LEX_C) $(TREE_SOURCE) $(BISON_OUT) $(SEMANTIC) $(INTER) $(MAIN) -o $(TARGET) $(LIBS) -Wunused-function

$(LEX_C): $(LEX_SOURCE) $(BISON_OUT)
	$(FLEX) $(LEX_SOURCE)

$(BISON_OUT): $(BISON_SOURCE)
	$(BISON) $(BISON_FLAGS) $(BISON_SOURCE)

clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_OUT)
