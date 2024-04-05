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
BISON_OUT = syntax.tab.c syntax.tab.h
SEMANTIC = semantic.c

all: $(TARGET)

$(TARGET): $(LEX_C)
	$(CC) $(CFLAGS) $(LEX_C) $(TREE_SOURCE) syntax.tab.c $(SEMANTIC) $(MAIN) -o $(TARGET) -ll
	# rm -f $(LEX_C)

$(LEX_C): $(LEX_SOURCE) $(BISON_OUT)
	$(FLEX) $(LEX_SOURCE) 

$(BISON_OUT): $(BISON_SOURCE)
	$(BISON) $(BISON_FLAGS) $(BISON_SOURCE)

clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_OUT)
