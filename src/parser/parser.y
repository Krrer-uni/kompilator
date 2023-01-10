
%code requires{
#include <string>
}

%code top {
  #include <assert.h>
  #include <ctype.h>  /* isdigit. */
  #include <stdio.h>  /* printf. */
  #include <stdlib.h> /* abort. */
  #include <string.h> /* strcmp. */
  #include <iostream>
  #include <numeric>
  #include <basic_blocks.h>
  #define YYDEBUG 1
  int yylex (void);
  void yyerror (char const *);
  extern int yy_flex_debug;
  void yyset_debug(int);
  std::string output;
  const unsigned int gf_base = 1234577;
  bool error_flag = false;
}


/* Generate YYSTYPE from the types used in %token and %type.  */
%define api.value.type union
%union {
   uint64_t num;
   std::string* identifier;
}

%token <uint64_t> num
%token <std::string*> identifier
%token PROCEDURE
%token IS
%token VAR
%token BEGIN_
%token END
%token PROGRAM
%token IF
%token THEN
%token ELSE
%token ENDIF
%token WHILE
%token DO
%token ENDWHILE
%token READ
%token WRITE
%token REPEAT
%token UNTIL
%token ASSIGN
%token NOTEQUAL
%token LESS
%token LESSEQUAL
%token GREATER
%token GREATEREQUAL
%token EQUAL
%token LPAREN
%token RPAREN
%token SEMICOL
%token COMMA
%token COLON
%token PLUS
%token MINUS
%token MULT
%token DIV
%token MOD
 %verbose
 %define parse.error detailed
 %define parse.trace
// %printer { fprintf (yyo, "%d", $$); } <long int>;

%% /* The grammar follows.  */

program_all: procedures main { std::cout << "program\n";}
;

procedures: procedures PROCEDURE proc_head IS VAR declarations BEGIN_ commands END { std::cout << "PROC \n";}
| procedures PROCEDURE proc_head IS BEGIN_ commands END { std::cout << "procedure no var" ; }
|
;

main: PROGRAM IS VAR declarations BEGIN_ commands END { std::cout << "MAIN" ; }
| PROGRAM IS BEGIN_ commands END { std::cout << "MAIN NO VAR" << std::endl; }
;

commands: commands command { std::cout << "command" << std::endl; }
| command { std::cout << "command" << std::endl; }
;

command: identifier ASSIGN expression SEMICOL { std::cout << "assign" << std::endl; }
| IF condition THEN commands ELSE commands ENDIF { std::cout << "IF ELSE" << std::endl; }
| IF condition THEN commands ENDIF { std::cout << "IF" << std::endl; }
| WHILE condition DO commands ENDWHILE { std::cout << "WHILE" << std::endl; }
| REPEAT commands UNTIL condition SEMICOL { std::cout << "REPEAT" << std::endl; }
| proc_head SEMICOL { std::cout << "procedure" << std::endl; }
| READ identifier SEMICOL { std::cout << "READ" << std::endl; }
| WRITE value SEMICOL { std::cout << "WRITE" << std::endl; }
;

proc_head: identifier LPAREN proc_declarations RPAREN { std::cout << "proc head" << std::endl; }
;

declarations: declarations COMMA identifier { std::cout << "Declaration" << std::endl; }
| identifier { std::cout << "Declaration" << std::endl; }
;

proc_declarations: proc_declarations COMMA identifier { std::cout << "PROC_Declaration \n";}
| identifier { std::cout << "PROC_Declaration \n";}
;


expression: value { std::cout << "VAL" << std::endl; }
| value PLUS value { std::cout << "plus" << std::endl; }
| value MINUS value { std::cout << "minus" << std::endl; }
| value MULT value { std::cout << "mult" << std::endl; }
| value DIV value { std::cout << "div" << std::endl; }
| value MOD value { std::cout << "mod" << std::endl; }

condition: value EQUAL value { std::cout << "EQUAL" << std::endl; }
| value NOTEQUAL value { std::cout << "NOTEQUAL" << std::endl; }
| value GREATER value { std::cout << "GREATER" << std::endl; }
| value LESS value { std::cout << "LESS" << std::endl; }
| value GREATEREQUAL value { std::cout << "GREATEREQUAL" << std::endl; }
| value LESSEQUAL value { std::cout << "LESSEQUAL" << std::endl; }

value: num { std::cout << "Numberr \n"; $$ = new }
| identifier { std::cout << "Identifier \n";}
;

%%

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
   fprintf (stderr, "%s\n", s);
}
