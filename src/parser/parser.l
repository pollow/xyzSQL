%{
extern "C" int yylex();
extern int yyerror( const char * );

#include "./parser/stmt.h"
#include "parser.tab.h"

#include <iostream>
using namespace std;
%}

%%
CREATE          { return CREATE;}
PRIMARY         { return PRIMARY; }
KEY             { return KEY; }
INDEX           { return INDEX; }
TABLE           { return TABLE;}
UNIQUE          { return UNIQUE; }
NOT             { return NOT; }
NULL            { return NULLX; }
DROP            { return DROP; }
INSERT          { return INSERT; }
QUIT            { return QUIT; }
EXEFILE         { return EXEFILE; }
TRANSACTION     { return TRANSACTION; }
ON              { return ON; }
COMMIT          { return COMMIT; }
ROLLBACK        { return ROLLBACK; }
SELECT          { return SELECT; }
DELETE          { return DELETE; }
FROM            { return FROM; }
WHERE           { return WHERE; }
AND             { return AND; }
INTO            { return INTO; }
VALUES          { return VALUES; }

CHAR            { return CHAR; }
INT             { return INT; }
FLOAT           { return FLOAT; }

create          { return CREATE;}
primary         { return PRIMARY; }
key             { return KEY; }
index           { return INDEX; }
table           { return TABLE;}
unique          { return UNIQUE; }
not             { return NOT; }
nullx           { return NULLX; }
drop            { return DROP; }
insert          { return INSERT; }
quit            { return QUIT; }
exefile         { return EXEFILE; }
transaction     { return TRANSACTION; }
on              { return ON; }
commit          { return COMMIT; }
rollback        { return ROLLBACK; }
select          { return SELECT; }
delete          { return DELETE; }
from            { return FROM; }
where           { return WHERE; }
and             { return AND; }
into            { return INTO; }
values          { return VALUES; }

char            { return CHAR; }
int             { return INT; }
float           { return FLOAT; }

[a-zA-Z][a-zA-Z0-9_]*       { yylval.strval = strdup(yytext); cout << "NAME: " << yytext << endl; return NAME;}

-?[0-9]+                    { yylval.intval = atoi(yytext); return INTNUM;}
-?[0-9]+"."[0-9]*           { yylval.floatval = atof(yytext); return FLOATNUM; }

'(\\.|''|[^'\n])*'          { yylval.strval = strdup(yytext); cout <<"String: " << yytext << endl; return STRING; }

"="     { yylval.intval = 1; return COMP; } 
">"     { yylval.intval = 2; return COMP; } 
"<"     { yylval.intval = 3; return COMP; } 
">="    { yylval.intval = 4; return COMP; } 
"<="    { yylval.intval = 5; return COMP; } 
"!="    { yylval.intval = 6; return COMP; } 

[ \t]                       {}
";" |
"," |
"(" |
")" |
"*" |
"."                 { return yytext[0]; }
.                   { yyerror(yytext); }
%%

