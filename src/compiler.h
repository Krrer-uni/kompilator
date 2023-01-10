//
// Created by krrer on 09.01.23.
//
#include <map>
#include "variables.h"
#include "basic_blocks.h"

#ifndef KOMPILATOR_SRC_COMPILER_H_
#define KOMPILATOR_SRC_COMPILER_H_

class Compiler {
 public:
  std::map<std::string, variable> _variable_map;
  int _debug_mode = 0;
  explicit Compiler(int debug_mode);
  void add_variable(const std::string &name, variable_type type);
  void handle_declaration(std::string name, variable_type type);
  std::shared_ptr<VAR_BLOCK> handle_variable(const std::string& name);
  std::shared_ptr<NUM_BLOCK> handle_literal(literal value);

};

#endif //KOMPILATOR_SRC_COMPILER_H_
