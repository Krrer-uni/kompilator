//
// Created by krrer on 19.12.22.
//
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

int main (int argc, char const* argv[]){
  if(argc < 2) {
    std::cout << "Too few arguments\n";
    return 1;
  }
  yyin = fopen(argv[1], "r");
  yyset_debug(1);
  yyparse();
  fclose(yyin);
}