/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 21 "src/parser/parser.y" /* yacc.c:1909  */

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

#line 113 "parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
