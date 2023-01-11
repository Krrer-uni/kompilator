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

  explicit Compiler(int debug_mode);
  void add_variable(const std::string &name, variable_type type);

  void handle_program();
  void handle_declaration(std::string name, variable_type type);
  VALUE_BLOCK* handle_variable(const std::string& name);
  VALUE_BLOCK* handle_literal(literal value);
  COMMAND_BLOCK* handle_write(VALUE_BLOCK* output);
  COMMAND_BLOCK* handle_read(std::string input_name);
  COMMANDS_BLOCK* handle_command(COMMANDS_BLOCK* commands_block, COMMAND_BLOCK* command);
  void handle_main(COMMANDS_BLOCK* commands_block);


 private:
  std::map<std::string, variable> _variable_map;
  int _debug_mode = 0;
  unsigned int _memory_count;
  MAIN_BLOCK* _main = nullptr;
  std::vector<std::string> main_code;
};

#endif //KOMPILATOR_SRC_COMPILER_H_
