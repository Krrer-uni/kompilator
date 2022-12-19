//
// Created by krrer on 19.12.22.
//
#include "lexer.hpp"
#include "parser.hpp"

int main (int argc, char const* argv[]){
  yyset_debug(0);
  return yyparse ();
}