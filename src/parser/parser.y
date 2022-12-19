%code top {
  #include <assert.h>
  #include <ctype.h>  /* isdigit. */
  #include <stdio.h>  /* printf. */
  #include <stdlib.h> /* abort. */
  #include <string.h> /* strcmp. */
  #include <string>
  #include <iostream>
  #include <numeric>
  #define OUTPUT_BUFFER 1000
  int yylex (void);
  void yyerror (char const *);
  extern int yy_flex_debug;
  void yyset_debug(int);
  std::string output;
  const unsigned int gf_base = 1234577;
  bool error_flag = false;
}

%define api.header.include {"parser.hpp"}



/* Generate YYSTYPE from the types used in %token and %type.  */
%define api.value.type union

%token <char> CHAR
%token  <long int> NUM "number"
%type  <long int> num expr
%left <long int> '+' '-'
%left <long int> '*' '/'
%precedence <long int> NEG
%precedence <long int> NUM_NEG
%right <long int> '^'


// %verbose
// %define parse.error detailed
// %define parse.trace
// %printer { fprintf (yyo, "%d", $$); } <long int>;

%% /* The grammar follows.  */
input:
  %empty
| input line
;

line:
  '\n'
| expr '\n'
| '#' commment '\n'
| error '\n'
;

expr:
  num
| expr '+' expr
| expr '-' expr
| expr '*' expr
| expr '/' expr
;



commment:
 %empty
| '+' commment
| '-' commment
| '*' commment
| '/' commment
| '^' commment
| '(' commment
| ')' commment
| NUM commment
| CHAR commment
| '#' commment
;

num:
  NUM {$$ = $1;}
| '-' NUM %prec NEG {$$ = -$2;}
;
%%

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
  // fprintf (stderr, "%s\n", s);
}
