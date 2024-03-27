# Makefile for compiling Flex-generated lexer

# Compiler
CC = gcc

# Flex compiler
FLEX = flex

# Flags for the compiler
CFLAGS = -Wall

# Target executable
TARGET = calc

# Source files
LEX_SOURCE = calc.l
LEX_C = lex.yy.c

all: $(TARGET)

$(TARGET): $(LEX_C)
	$(CC) $(CFLAGS) $(LEX_C) -o $(TARGET) -ll
	# rm -f $(LEX_C)

$(LEX_C): $(LEX_SOURCE)
	$(FLEX) $(LEX_SOURCE)

clean:
	rm -f $(TARGET) $(LEX_C)
