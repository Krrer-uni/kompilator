%option noyywrap
%option yylineno
%{
#include "parser.hpp"
#include <string>
int yylex();
%}
%%
[ \t]+ 	;
(\\[ \t]*\n)+ ;
[0-9]+  	{ yylval.NUM = atoi(yytext); return NUM; }
\+	{ return '+'; }
-	{ return '-'; }
\*	{ return '*'; }
\/	{ return '/'; }
\^  { return '^'; }
\(	{ return '('; }
\)	{ return ')'; }
\n  { return '\n'; }
#   { return '#'; }
.   { return CHAR; }
%%