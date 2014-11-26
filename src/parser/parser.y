%{

extern "C" int yylex();
extern char *yytext;

void yyerror(const char *);
extern int yylineno;

#include "src/parser/stmt.h"
#include <iostream>
#include <cstring>
#include <algorithm>

// extern function and enum stmt type

extern void xyzsql_emit_stmt(stmt_type, statement *);

using namespace std;

%}

%union {
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
};

%token <strval> NAME STRING
%token <intval> COMP INTNUM
%token <floatval> FLOATNUM 
%token CREATE TABLE PRIMARY KEY INDEX INT CHAR FLOAT UNIQUE NOT NULLX ON AND 
%token DROP QUIT EXEFILE TRANSACTION SELECT FROM WHERE DELETE ROLLBACK COMMIT
%token INSERT INTO VALUES

%type <intval> column_atts opt_length data_type
%type <colval> create_def
%type <collist> create_col_list
%type <attrval> attribute
%type <attrlist> select_list
%type <namelist> from_list
%type <condlist> conditions;
%type <condval> condition;
%type <valuelist> value_list;
%type <valueval> value;

%type <insert_stmt_val>             insert_stmt;
%type <create_table_stmt_val>       create_table_stmt;
%type <create_index_stmt_val>       create_index_stmt;
%type <select_stmt_val>             select_stmt;
%type <drop_index_stmt_val>         drop_index_stmt;
%type <drop_table_stmt_val>         drop_table_stmt;
%type <delete_stmt_val>             delete_stmt;
%type <exefile_stmt_val>            exefile

%%

stmt_list: stmt {  }
         | stmt stmt_list { }
;

stmt    : create_table_stmt ';' { xyzsql_emit_stmt(stmt_type::_create_table_stmt,   $1); }
        | select_stmt       ';' { xyzsql_emit_stmt(stmt_type::_select_stmt,         $1); }
        | create_index_stmt ';' { xyzsql_emit_stmt(stmt_type::_create_index_stmt,   $1); }
        | insert_stmt       ';' { xyzsql_emit_stmt(stmt_type::_insert_stmt,         $1); }
        | drop_index_stmt   ';' { xyzsql_emit_stmt(stmt_type::_drop_index_stmt,     $1); }
        | drop_table_stmt   ';' { xyzsql_emit_stmt(stmt_type::_drop_table_stmt,     $1); }
        | delete_stmt       ';' { xyzsql_emit_stmt(stmt_type::_delete_stmt,         $1); }
        | exefile           ';' { xyzsql_emit_stmt(stmt_type::_exefile_stmt,        $1); }
        | quit              ';' { xyzsql_emit_stmt(stmt_type::_quit_stmt,           nullptr); }
        | transaction_on    ';' { xyzsql_emit_stmt(stmt_type::_transaction_stmt,    nullptr); }
        | commit            ';' { xyzsql_emit_stmt(stmt_type::_commit_stmt,         nullptr); }
        | rollback          ';' { xyzsql_emit_stmt(stmt_type::_rollback_stmt,       nullptr); }
;

/* create statements */

create_table_stmt : CREATE TABLE NAME '(' create_col_list ')' { $$ = new create_table_stmt($3, $5); free($3);}
;

data_type: INT                      { $$ = table_column::INTTYPE; }
         | CHAR                     { $$ = table_column::CHARTYPE; }
         | FLOAT                    { $$ = table_column::FLOATTYPE; }
;

column_atts :               { $$ = 0; }
            | PRIMARY KEY   { $$ = table_column::primary_attr; }
            | INDEX         { $$ = table_column::index_attr; }
            | NOT NULLX     { $$ = table_column::notnull_attr; }
            | UNIQUE        { $$ = table_column::unique_attr; }
;

opt_length  :                   { $$ = 4; }
            | '(' INTNUM ')'    { $$ = $2; }

create_def  : NAME data_type opt_length column_atts     { $$ = new table_column($1, $2, $3, $4); free($1);}
            | PRIMARY KEY '(' NAME ')'                  {}
            | INDEX '(' NAME ')'                        {}
;

create_col_list :                                   {  }
                | create_def                        { $$ = new vector<table_column *>(); $$->push_back($1);}
                | create_def ',' create_col_list    { $3->push_back($1); $$ = $3; }

/* create index */

create_index_stmt   : CREATE INDEX attribute     { $$ = new create_index_stmt($3); }
;

/* drop table */

drop_table_stmt : DROP TABLE NAME { $$ = new drop_table_stmt($3); free($3);}
;

/* drop index */

drop_index_stmt : DROP INDEX attribute   { $$ = new drop_index_stmt($3);}
;

/* select */

attribute   : NAME '.' NAME     { $$ = new attribute($1, $3); free($1); free($3);}
;

select_list : '*'                           { $$ = new vector<attribute *>(); }
            | attribute                     { $$ = new vector<attribute *>(); $$->push_back($1);}
            | attribute ',' select_list     { $$ = $3; $$->push_back($1); }
;

from_list   : NAME                  { $$ = new vector<string *>(); $$->push_back(new string($1)); free($1);}
            | NAME ',' from_list    { $$ = $3; $$->push_back(new string($1)); free($1);}
;

conditions  : condition                 { $$ = new vector<condition *>(); $$->push_back($1); }
            | condition AND conditions  { $$ = $3; $$->push_back($1); }

condition   : attribute COMP attribute          { $$ = new condition($1, $3, $2); }
            | attribute COMP STRING             { $$ = new condition($1, $3, $2); }
            | attribute COMP INTNUM             { $$ = new condition($1, $3, $2); }
            | attribute COMP FLOATNUM           { $$ = new condition($1, $3, $2); }
             /* | attribute IN '(' select_stmt ')'  { $$ = new condition($1, $4, $2); } */
;

select_stmt : SELECT select_list FROM from_list                     { $$ = new select_stmt($2, $4, new vector<condition *>()); }
            | SELECT select_list FROM from_list WHERE conditions    { $$ = new select_stmt($2, $4, $6); }
;

/* DELETE */

delete_stmt : DELETE FROM NAME                  { $$ = new delete_stmt($3, new vector<condition *>); free($3);}
            | DELETE FROM NAME WHERE conditions { $$ = new delete_stmt($3, $5); free($3);}
;

/* INSERT */

value       : INTNUM            { $$ = new record_value($1); }
            | FLOATNUM          { $$ = new record_value($1); }
            | STRING            { $$ = new record_value($1); }
;

value_list  : value                 { $$ = new vector<record_value>(); $$->push_back(*($1)); delete $1;}
            | value ',' value_list  { $3->push_back(*($1)); delete $1; $$ = $3;}
;

insert_stmt: INSERT INTO NAME VALUES '(' value_list ')'    { $$ = new insert_stmt($3, $6); free($3);}

/* Transaction */

transaction_on  : TRANSACTION ON {}
;

commit      : COMMIT {}
;

rollback    : ROLLBACK {}
;

/* repl */

quit : QUIT {}
;

exefile : EXEFILE STRING    {$$ = new exefile_stmt(string($2)); delete[] $2;}
;

%%

void yyerror(const char *s) {
    cout << "Got An Error: " << yytext << endl;
}

