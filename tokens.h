/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_TOKENS_H_INCLUDED
# define YY_YY_TOKENS_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KW_INPUT = 258,
    KW_TRUE = 259,
    KW_FALSE = 260,
    KW_BREAK = 261,
    KW_CONTINUE = 262,
    KW_CLASS = 263,
    KW_DEF = 264,
    KW_PRINT = 265,
    KW_IF = 266,
    KW_ELIF = 267,
    KW_ELSE = 268,
    KW_FOR = 269,
    KW_WHILE = 270,
    KW_RETURN = 271,
    TK_INDENT = 272,
    TK_DEDENT = 273,
    TK_NUM = 274,
    TK_ID = 275,
    STRING_LITERAL = 276,
    OP_GREATEREQUAL = 277,
    OP_LESSEQUAL = 278,
    OP_EQUAL = 279,
    OP_NOTEQUAL = 280,
    OP_NOT = 281,
    OP_OR = 282,
    OP_AND = 283
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 22 "grammar.y" /* yacc.c:1909  */

	
	Class* class_t;
	GlobalVariable* globalVariable_t;
	Parameter* param_t;
	DeclaredParameter* dparam_t;
	Method* method_t;

	char* str_t;
	int num_t;

	Expression* expression_t;
	Statement* statement_t;


#line 99 "tokens.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKENS_H_INCLUDED  */
