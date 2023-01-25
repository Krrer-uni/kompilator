%option noyywrap
%option yylineno
%{
#include "parser.hpp"
#include <string>
int yylex();
%}

%x IN_COMMENT

%%
<INITIAL>{
[ \t]+ 	;
[0-9]+  	        {yylval.num = std::stoull(yytext); return num;}
"PROCEDURE"         {return PROCEDURE;}
"IS"                {return IS;}
"VAR"               {return VAR;}
"BEGIN"             {return BEGIN_;}
"END"               {return END;}
"PROGRAM"           {return PROGRAM;}
"IF"                {return IF;}
"THEN"              {return THEN;}
"ELSE"              {return ELSE;}
"ENDIF"             {return ENDIF;}
"WHILE"             {return WHILE;}
"DO"                {return DO;}
"ENDWHILE"          {return ENDWHILE;}
"READ"              {return READ;}
"WRITE"             {return WRITE;}
"REPEAT"            {return REPEAT;}
"UNTIL"             {return UNTIL;}
"+"                 {return  PLUS ;}
"-"                 {return  MINUS ;}
"*"                 {return  MULT ;}
"/"                 {return  DIV ;}
"%"                 {return  MOD ;}
":="                {return ASSIGN;}
"="                 {return EQUAL;}
"!="                {return NOTEQUAL;}
"<"                 {return LESS;}
"<="                {return LESSEQUAL;}
">"                 {return GREATER;}
">="                {return GREATEREQUAL;}
"("                 {return  LPAREN ;}
")"                 {return  RPAREN ;}
";"                 {return  SEMICOL ;}
","                 {return  COMMA ;}
":"                 {return  COLON ;}
[_a-z]+             {yylval.identifier = new std::string(yytext); return identifier;}
"["                 BEGIN(IN_COMMENT);
\n+                 ;
}
<IN_COMMENT>{
"]"                 BEGIN(INITIAL);
[^\n\]]+              ;
}
%%