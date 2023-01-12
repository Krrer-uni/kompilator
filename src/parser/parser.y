
%code requires{
#include <string>
#include "../basic_blocks/basic_blocks.h"


}

%code top {
  #include <assert.h>
  #include <ctype.h>  /* isdigit. */
  #include <stdio.h>  /* printf. */
  #include <stdlib.h> /* abort. */
  #include <string.h> /* strcmp. */
  #include <iostream>
  #include <numeric>
  #include <map>
  #include <memory>
  #include "../compiler.h"
  #define YYDEBUG 1
  int yylex (void);
  void yyerror (char const *);
  extern int yy_flex_debug;
  void yyset_debug(int);
  std::string output;
  const unsigned int gf_base = 1234577;
  bool error_flag = false;
  int compiler_debug_mode = 1;
  auto compiler = new Compiler(compiler_debug_mode);
}


/* Generate YYSTYPE from the types used in %token and %type.  */

%union {
   uint64_t num;
   std::string* identifier;
   VALUE_BLOCK* value_type;
   COMMAND_BLOCK* command_type;
   COMMANDS_BLOCK* commands_type;
   MAIN_BLOCK* main_type;
	EXPRESSION_BLOCK* expression_type;
}

%token <num> num
%token <identifier> identifier
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

%type <value_type> value
%type <command_type> command
%type <commands_type> commands
%type <main_type> main
%type <expression_type> expression
 %verbose
 %define parse.error detailed
 %define parse.trace


%% /* The grammar follows.  */

program_all: procedures main { compiler->handle_program();}
;

procedures: procedures PROCEDURE proc_head IS VAR declarations BEGIN_ commands END { std::cout << "PROC \n";}
| procedures PROCEDURE proc_head IS BEGIN_ commands END { std::cout << "procedure no var" ; }
|
;

main: PROGRAM IS VAR declarations BEGIN_ commands END { compiler->handle_main($6);}
| PROGRAM IS BEGIN_ commands END { compiler->handle_main($4); }
;

commands: commands command { $$ = compiler->handle_command($1, $2); }
| command { $$ = compiler->handle_command(nullptr, $1);}
;

command: identifier ASSIGN expression SEMICOL { $$ = compiler->handle_assign(*$1,$3);}
| IF condition THEN commands ELSE commands ENDIF { std::cout << "IF ELSE" << std::endl; }
| IF condition THEN commands ENDIF { std::cout << "IF" << std::endl; }
| WHILE condition DO commands ENDWHILE { std::cout << "WHILE" << std::endl; }
| REPEAT commands UNTIL condition SEMICOL { std::cout << "REPEAT" << std::endl; }
| proc_head SEMICOL { std::cout << "procedure" << std::endl; }
| READ identifier SEMICOL { $$ =compiler->handle_read(*$2);}
| WRITE value SEMICOL {$$ =compiler->handle_write($2);}
;

proc_head: identifier LPAREN proc_declarations RPAREN { std::cout << "proc head" << std::endl; }
;

declarations: declarations COMMA identifier { compiler->handle_declaration(*$3, value_var); }
| identifier {compiler->handle_declaration(*$1, value_var); }
;

proc_declarations: proc_declarations COMMA identifier { std::cout << "PROC_Declaration \n";}
| identifier { std::cout << "PROC_Declaration \n";}
;


expression: value { $$ = compiler->handle_expression( basic_blocks_types::EXP_VAL, $1, nullptr);}
| value PLUS value { $$ = compiler->handle_expression( basic_blocks_types::EXP_ADD, $1, $3); }
| value MINUS value {$$ = compiler->handle_expression( basic_blocks_types::EXP_SUB, $1, $3); }
| value MULT value { $$ = compiler->handle_expression( basic_blocks_types::EXP_MUL, $1, $3); }
| value DIV value { $$ = compiler->handle_expression( basic_blocks_types::EXP_DIV, $1, $3); }
| value MOD value { $$ = compiler->handle_expression( basic_blocks_types::EXP_MOD, $1, $3); }

condition: value EQUAL value { std::cout << "EQUAL" << std::endl; }
| value NOTEQUAL value { std::cout << "NOTEQUAL" << std::endl; }
| value GREATER value { std::cout << "GREATER" << std::endl; }
| value LESS value { std::cout << "LESS" << std::endl; }
| value GREATEREQUAL value { std::cout << "GREATEREQUAL" << std::endl; }
| value LESSEQUAL value { std::cout << "LESSEQUAL" << std::endl; }

value: num {$$ = compiler->handle_literal($1);}
| identifier {$$ = compiler->handle_variable(*$1);}
;

%%

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
   fprintf (stderr, "%s\n", s);
}
