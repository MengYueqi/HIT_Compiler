# Makefile for compiling Flex-generated lexer

# Compiler
CC = gcc

# Flex compiler
FLEX = flex

# Bison compiler
BISON = bison

# Flags for the compiler
CFLAGS = -Wall

# Target executable
TARGET = parser

# Bison source
BISON_FLAGS = -d

# Source files
LEX_SOURCE = lex.l
LEX_C = lex.yy.c
MAIN = main.c
TREE_SOURCE = tree.c
BISON_SOURCE = syntax.y
BISON_OUT = syntax.tab.c
SEMANTIC = semantic.c

# 获取系统信息
ARCH = "`uname -s`"
MACOS = "Darwin"
LINUX = "Linux"

all: build

build:
	@if [ $(ARCH) = $(MACOS) ]; \
	then \
		$(TARGET): $(LEX_C) $(SEMANTIC) $(BISON_OUT) $(MAIN) $(TREE_SOURCE) ;\
			$(CC) $(CFLAGS) $(LEX_C) $(TREE_SOURCE) $(BISON_OUT) $(SEMANTIC) $(MAIN) -o $(TARGET) -ll -ly ;\
			rm -f $(LEX_C) ;\
		$(LEX_C): $(LEX_SOURCE) $(BISON_OUT) ;\
			$(FLEX) $(LEX_SOURCE) ;\
		$(BISON_OUT): $(BISON_SOURCE) ;\
			$(BISON) $(BISON_FLAGS) $(BISON_SOURCE) ;\
	elif [ $(ARCH) = $(LINUX) ]; \
	then \
		$(TARGET): $(LEX_C) $(SEMANTIC) $(BISON_OUT) $(MAIN) $(TREE_SOURCE) ;\
			$(CC) $(CFLAGS) $(LEX_C) $(TREE_SOURCE) $(BISON_OUT) $(SEMANTIC) $(MAIN) -o $(TARGET) -lfl ;\
			rm -f $(LEX_C) ;\
		$(LEX_C): $(LEX_SOURCE) $(BISON_OUT) ;\
			$(FLEX) $(LEX_SOURCE) ;\
		$(BISON_OUT): $(BISON_SOURCE) ;\
			$(BISON) $(BISON_FLAGS) $(BISON_SOURCE) ;\
	else \
		@echo "Unsupported operating system: $(ARCH)" ;\
	fi

clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_OUT)


	else \
		@echo "Unsupported operating system: $(ARCH)" ;\
	fi

clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_OUT)
