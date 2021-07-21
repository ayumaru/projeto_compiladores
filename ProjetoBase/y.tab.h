/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    PROGRAM = 258,
    ABRE_PARENTESES = 259,
    FECHA_PARENTESES = 260,
    VIRGULA = 261,
    PONTO_E_VIRGULA = 262,
    DOIS_PONTOS = 263,
    PONTO = 264,
    T_BEGIN = 265,
    T_END = 266,
    VAR = 267,
    IDENT = 268,
    ATRIBUICAO = 269,
    ABRE_COLCHETES = 270,
    FECHA_COLCHETES = 271,
    ABRE_CHAVES = 272,
    FECHA_CHAVES = 273,
    MAIOR = 274,
    MAIOR_IGUAL = 275,
    MENOR = 276,
    MENOR_IGUAL = 277,
    IGUAL = 278,
    DIFERENTE = 279,
    SOMA = 280,
    SUBTRACAO = 281,
    MULTIPLICACAO = 282,
    DIVISAO = 283,
    AND = 284,
    OR = 285,
    LABEL = 286,
    TYPE = 287,
    ARRAY = 288,
    OF = 289,
    PROCEDURE = 290,
    FUNCTION = 291,
    IF = 292,
    ELSE = 293,
    WHILE = 294,
    DO = 295,
    NOT = 296,
    THEN = 297
  };
#endif
/* Tokens.  */
#define PROGRAM 258
#define ABRE_PARENTESES 259
#define FECHA_PARENTESES 260
#define VIRGULA 261
#define PONTO_E_VIRGULA 262
#define DOIS_PONTOS 263
#define PONTO 264
#define T_BEGIN 265
#define T_END 266
#define VAR 267
#define IDENT 268
#define ATRIBUICAO 269
#define ABRE_COLCHETES 270
#define FECHA_COLCHETES 271
#define ABRE_CHAVES 272
#define FECHA_CHAVES 273
#define MAIOR 274
#define MAIOR_IGUAL 275
#define MENOR 276
#define MENOR_IGUAL 277
#define IGUAL 278
#define DIFERENTE 279
#define SOMA 280
#define SUBTRACAO 281
#define MULTIPLICACAO 282
#define DIVISAO 283
#define AND 284
#define OR 285
#define LABEL 286
#define TYPE 287
#define ARRAY 288
#define OF 289
#define PROCEDURE 290
#define FUNCTION 291
#define IF 292
#define ELSE 293
#define WHILE 294
#define DO 295
#define NOT 296
#define THEN 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
