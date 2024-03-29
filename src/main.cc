//
// Created by krrer on 19.12.22.
//
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.h"

int main (int argc, char const* argv[]){
  if(argc < 2) {
    std::cout << "Too few arguments\n";
    return 1;
  }
  extern Compiler* compiler;
  Compiler::set_log_level(0);
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    std::cout << "FATAL ERROR: file with given name does not exist: " + std::string(argv[1]) + "\n";
  }
  auto output_file = fopen(argv[2],"w");
  compiler->set_output(output_file);
  yyset_debug(1);
  yyparse();
  fclose(yyin);
}