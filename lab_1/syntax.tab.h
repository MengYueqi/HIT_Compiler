/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEMI = 258,
     COMMA = 259,
     ASSIGNOP = 260,
     RELOP = 261,
     PLUS = 262,
     MINUS = 263,
     STAR = 264,
     DIV = 265,
     AND = 266,
     OR = 267,
     DOT = 268,
     NOT = 269,
     TYPE = 270,
     LP = 271,
     RP = 272,
     LB = 273,
     RB = 274,
     LC = 275,
     RC = 276,
     STRUCT = 277,
     RETURN = 278,
     IF = 279,
     ELSE = 280,
     WHILE = 281,
     TIMES = 282,
     LPAREN = 283,
     NUMBER = 284,
     RPAREN = 285,
     DIVIDE = 286
   };
#endif
/* Tokens.  */
#define SEMI 258
#define COMMA 259
#define ASSIGNOP 260
#define RELOP 261
#define PLUS 262
#define MINUS 263
#define STAR 264
#define DIV 265
#define AND 266
#define OR 267
#define DOT 268
#define NOT 269
#define TYPE 270
#define LP 271
#define RP 272
#define LB 273
#define RB 274
#define LC 275
#define RC 276
#define STRUCT 277
#define RETURN 278
#define IF 279
#define ELSE 280
#define WHILE 281
#define TIMES 282
#define LPAREN 283
#define NUMBER 284
#define RPAREN 285
#define DIVIDE 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 7 "syntax.y"
{
    Node node;
}
/* Line 1529 of yacc.c.  */
#line 115 "syntax.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

