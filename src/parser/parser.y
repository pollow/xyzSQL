%{

extern "C" int yylex();

int yyerror(const char *);
extern int yylineno;

#include "./parser/stmt.h"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

%}

%union {
    char *strval;
    int intval;
    float floatval;
    table_info *tableval;
    table_column *colval;
    vector<table_column *> *collist;
    attribute *attrval;
    vector<attribute *> *attrlist;
    vector<string *> *namelist;
    condition *condval;
    vector<condition *> *condlist;
};

%token <strval> NAME STRING
%token <intval> COMP INTNUM
%token <floatval> FLOATNUM 
%token CREATE TABLE PRIMARY KEY INDEX INT CHAR FLOAT UNIQUE NOT NULLX ON AND 
%token DROP QUIT EXEFILE TRANSACTION SELECT FROM WHERE DELETE ROLLBACK COMMIT
%token INSERT INTO VALUES

%type <intval> column_atts opt_length data_type
%type <tableval> create_stmt
%type <colval> create_def
%type <collist> create_col_list
%type <attrval> attribute
%type <attrlist> select_list
%type <namelist> from_list
%type <condlist> condition;

%%

stmt_list: stmt ';' {  }
         | stmt ';' stmt_list { cout << "Got a create stmt!" << endl; }
;

stmt: create_stmt {cout << "Got a create stmt!" << endl;}
    | select_stmt {cout << "Got a select stmt!" << endl;}
    | create_index_stmt {}
    | insesrt_stmt      {}
    | drop_index_stmt   {}
    | drop_table_stmt   {}
    | transaction_on    {}
    | commit            {}
    | delete_stmt       {}
    | rollback          {}
    | quit              {}
    | exefile           {}
;

/* create statements */

create_stmt : CREATE TABLE NAME '(' create_col_list ')' { $$ = new table_info($3, $5);/*cout << "***" << $3 << endl;*/ }
;

data_type: INT                      { $$ = table_column::INTTYPE; cout << "INT" << endl; }
         | CHAR                     { $$ = table_column::CHARTYPE; cout << "CHAR" << endl; }
         | FLOAT                    { $$ = table_column::FLOATTYPE; cout << "FLOAT" << endl; }
;

column_atts :               { $$ = 0; cout << "No Attribute." << endl; }
            | PRIMARY KEY   { $$ = table_column::primary_attr; cout << "PRIMARY" << endl; }
            | INDEX         { $$ = table_column::index_attr; cout << "INDEX" << endl; }
            | NOT NULLX     { $$ = table_column::notnull_attr; cout << "NOT NULL" << endl; }
            | UNIQUE        { $$ = table_column::unique_attr; cout << "UNIQUE" << endl; }
;

create_def  : NAME data_type opt_length column_atts     { $$ = new table_column($1, $2, $3, $4); cout << "This column is: " << $1 << " " << $2 << " " << $3 << endl; }
            | PRIMARY KEY '(' NAME ')'                  {}
            | INDEX '(' NAME ')'                        {}
;

opt_length  :                   { $$ = 0; }
            | '(' INTNUM ')'    { $$ = INTNUM; }

create_col_list :  {}
                | create_def                        { $$ = new vector<table_column *>(); $$->push_back($1);}
                | create_def ',' create_col_list    { $3->push_back($1); $$ = $3; }

/* create index */

create_index_stmt   : CREATE INDEX NAME ON NAME '(' NAME ')' {}
;

/* drop table */

drop_table_stmt : DROP TABLE NAME {}
;

/* drop index */

drop_index_stmt : DROP INDEX NAME {}
;

/* select */

attribute   : NAME '.' NAME     { $$ = new attribute($1, $3); cout << "Attribute!" << endl; }
;

select_list : '*'                           { $$ = new vector<attribute *>(); $$->push_back(new attribute("*", "*"));}
            | attribute                     { $$ = new vector<attribute *>(); $$->push_back($1);}
            | attribute ',' select_list     { $$ = $3; $$->push_back($1); }
;

from_list   : NAME                  { $$ = new vector<string *>(); $$->push_back(new string($1)); }
            | NAME ',' from_list    { $$ = $3; $$->push_back(new string($1)); }
;

condition   : condition AND condition           { $$ = new vector<condition *>($1->size() + $3->size()); merge($1->begin(), $1->end(), $3->begin(), $3->end(), $$->begin()); delete $1; delete $3;}
            | attribute COMP attribute          { $$ = new vector<condition *>(); $$->push_back(new condition($1, $3, $2)); }
            | attribute COMP STRING             { $$ = new vector<condition *>(); $$->push_back(new condition($1, $3, $2)); }
            | attribute COMP INTNUM             { $$ = new vector<condition *>(); $$->push_back(new condition($1, $3, $2)); }
            | attribute COMP FLOATNUM           { $$ = new vector<condition *>(); $$->push_back(new condition($1, $3, $2)); }
             /* | attribute IN '(' select_stmt ')'  { $$ = new condition($1, $4, $2); } */
;

select_stmt : SELECT select_list FROM NAME                      { cout << "SELECT STMT" << endl; select_stmt tmp($2, new vector<string *>(1, new string($4)), new vector<condition *>()); tmp.print();}
            | SELECT select_list FROM from_list WHERE condition { select_stmt tmp($2, $4, $6); tmp.print();}
;

/* DELETE */

delete_stmt : DELETE FROM NAME                  {}
            | DELETE FROM NAME WHERE condition  {}
;

/* INSERT */

value       : INTNUM            { cout << "int: " << $1; }
            | FLOATNUM          { cout << "float: " << $1; }
            | STRING            { cout << "string: " << $1; }
;

value_list  : value                 {}
            | value_list ',' value  {}
;

insesrt_stmt: INSERT INTO NAME VALUES '(' value_list ')'    { cout << endl << "record inserted" << endl; }

/* Transaction */

transaction_on  : TRANSACTION ON {}
;

commit  : COMMIT {}
;

rollback    : ROLLBACK {}
;

/* repl */

quit : QUIT {}
;

exefile : EXEFILE {}
;

%%

int yyerror(const char *s) {
    cout << "Got An Error:" << yylineno << endl;
}

