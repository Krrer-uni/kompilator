
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
  auto compiler = new Compiler();
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
   CONDITION_BLOCK* condition_type;
   std::vector<std::string>* proc_declaration_type;
   std::vector<variable>* proc_head_type;
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
%type <condition_type> condition
%type <proc_declaration_type> proc_declarations
%type <proc_head_type> proc_head
%type <command_type> proc_use
 %verbose
 %define parse.error detailed
 %define parse.trace


%% /* The grammar follows.  */

program_all: procedures main { compiler->handle_program();}
;

procedures: procedures PROCEDURE proc_head IS VAR declarations BEGIN_ commands END { compiler->handle_procedure_definition($8,$3);}
| procedures PROCEDURE proc_head IS BEGIN_ commands END {  compiler->handle_procedure_definition($6,$3); }
| {std::cout << "end of proc declarations \n";}
;

main: PROGRAM IS VAR declarations BEGIN_ commands END { compiler->handle_main($6);}
| PROGRAM IS BEGIN_ commands END { compiler->handle_main($4); }
;

commands: commands command { $$ = compiler->handle_command($1, $2); }
| command { $$ = compiler->handle_command(nullptr, $1);}
;

command: identifier ASSIGN expression SEMICOL { $$ = compiler->handle_assign(*$1,$3);}
| IF condition THEN commands ELSE commands ENDIF { $$ = compiler->handle_if_else($2,$4,$6); }
| IF condition THEN commands ENDIF { $$ = compiler->handle_if($2,$4);}
| WHILE condition DO commands ENDWHILE { $$ = compiler->handle_while($2,$4);}
| REPEAT commands UNTIL condition SEMICOL { $$ = compiler->handle_repeat($4,$2); }
| proc_use SEMICOL {$$ = $1; }
| READ identifier SEMICOL { $$ =compiler->handle_read(*$2);}
| WRITE value SEMICOL {$$ =compiler->handle_write($2);}
;

proc_head: identifier LPAREN proc_declarations RPAREN {$$ =  compiler->handle_proc_head(*$1,$3) ;}
;

proc_use: identifier LPAREN proc_declarations RPAREN {$$ = compiler->handle_proc_use(*$1, $3);}
;

declarations: declarations COMMA identifier { compiler->handle_declaration(*$3, value_var); }
| identifier { compiler->handle_declaration(*$1, value_var); }
;

proc_declarations: proc_declarations COMMA identifier {$$ = compiler->handle_proc_declaration(*$3, $1);}
| identifier {$$ = compiler->handle_proc_declaration(*$1, nullptr);}
;


expression: value { $$ = compiler->handle_expression( basic_blocks_types::EXP_VAL, $1, nullptr);}
| value PLUS value { $$ = compiler->handle_expression( basic_blocks_types::EXP_ADD, $1, $3); }
| value MINUS value {$$ = compiler->handle_expression( basic_blocks_types::EXP_SUB, $1, $3); }
| value MULT value { $$ = compiler->handle_expression( basic_blocks_types::EXP_MUL, $1, $3); }
| value DIV value { $$ = compiler->handle_expression( basic_blocks_types::EXP_DIV, $1, $3); }
| value MOD value { $$ = compiler->handle_expression( basic_blocks_types::EXP_MOD, $1, $3); }

condition: value EQUAL value { $$ = compiler->handle_condition(basic_blocks_types::CON_EQ, $1,$3); }
| value NOTEQUAL value { $$ = compiler->handle_condition(basic_blocks_types::CON_NEQ, $1,$3); }
| value GREATER value { $$ = compiler->handle_condition(basic_blocks_types::CON_GR, $1,$3); }
| value LESS value { $$ = compiler->handle_condition(basic_blocks_types::CON_LS, $1,$3); }
| value GREATEREQUAL value { $$ = compiler->handle_condition(basic_blocks_types::CON_GRE, $1,$3); }
| value LESSEQUAL value { $$ = compiler->handle_condition(basic_blocks_types::CON_LSE, $1,$3); }

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
