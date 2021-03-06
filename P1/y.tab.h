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
    Name = 258,
    Float = 259,
    Int = 260,
    String = 261,
    CREATE = 262,
    TABLE = 263,
    TOKENHEAP = 264,
    TOKENSORTED = 265,
    ON = 266,
    TOKENINTEGER = 267,
    TOKENDOUBLE = 268,
    TOKENSTRING = 269,
    INSERT = 270,
    INTO = 271,
    DROP = 272,
    SET = 273,
    OUTPUT = 274,
    QUIT = 275,
    SELECT = 276,
    GROUP = 277,
    DISTINCT = 278,
    BY = 279,
    FROM = 280,
    WHERE = 281,
    SUM = 282,
    AS = 283,
    AND = 284,
    OR = 285
  };
#endif
/* Tokens.  */
#define Name 258
#define Float 259
#define Int 260
#define String 261
#define CREATE 262
#define TABLE 263
#define TOKENHEAP 264
#define TOKENSORTED 265
#define ON 266
#define TOKENINTEGER 267
#define TOKENDOUBLE 268
#define TOKENSTRING 269
#define INSERT 270
#define INTO 271
#define DROP 272
#define SET 273
#define OUTPUT 274
#define QUIT 275
#define SELECT 276
#define GROUP 277
#define DISTINCT 278
#define BY 279
#define FROM 280
#define WHERE 281
#define SUM 282
#define AS 283
#define AND 284
#define OR 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 39 "Parser.y" /* yacc.c:1909  */

	struct CreateTable *myCreateTable;
  	struct AttrList *myAttrList;
  	struct InsertFile *myInsertFile;
	struct Attr *myAttr;
	struct AndList *final;
 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	char *actualChars;
	char whichOne;

#line 132 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
