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
     NAME = 258,
     STRING = 259,
     COMP = 260,
     INTNUM = 261,
     FLOATNUM = 262,
     CREATE = 263,
     TABLE = 264,
     PRIMARY = 265,
     KEY = 266,
     INDEX = 267,
     INT = 268,
     CHAR = 269,
     FLOAT = 270,
     UNIQUE = 271,
     NOT = 272,
     NULLX = 273,
     ON = 274,
     AND = 275,
     DROP = 276,
     QUIT = 277,
     EXEFILE = 278,
     TRANSACTION = 279,
     SELECT = 280,
     FROM = 281,
     WHERE = 282,
     DELETE = 283,
     ROLLBACK = 284,
     COMMIT = 285,
     INSERT = 286,
     INTO = 287,
     VALUES = 288
   };
#endif
/* Tokens.  */
#define NAME 258
#define STRING 259
#define COMP 260
#define INTNUM 261
#define FLOATNUM 262
#define CREATE 263
#define TABLE 264
#define PRIMARY 265
#define KEY 266
#define INDEX 267
#define INT 268
#define CHAR 269
#define FLOAT 270
#define UNIQUE 271
#define NOT 272
#define NULLX 273
#define ON 274
#define AND 275
#define DROP 276
#define QUIT 277
#define EXEFILE 278
#define TRANSACTION 279
#define SELECT 280
#define FROM 281
#define WHERE 282
#define DELETE 283
#define ROLLBACK 284
#define COMMIT 285
#define INSERT 286
#define INTO 287
#define VALUES 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "./parser/parser.y"
{
    char *strval;
    int intval;
    float floatval;
    table_column *colval;
    vector<table_column *> *collist;
    attribute *attrval;
    vector<attribute *> *attrlist;
    vector<string *> *namelist;
    vector<record_value> *valuelist;
    condition *condval;
    vector<condition *> *condlist;
    record_value *valueval;
    
    // stmt types
    insert_stmt *insert_stmt_val;
    create_table_stmt *create_table_stmt_val;
    create_index_stmt *create_index_stmt_val;
    select_stmt *select_stmt_val;
    drop_index_stmt *drop_index_stmt_val;
    drop_table_stmt *drop_table_stmt_val;
    delete_stmt *delete_stmt_val;
    exefile_stmt *exefile_stmt_val;
}
/* Line 1529 of yacc.c.  */
#line 140 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

