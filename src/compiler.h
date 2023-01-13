//
// Created by krrer on 09.01.23.
//
#include <map>
#include "variables.h"
#include "basic_blocks/basic_blocks.h"


#ifndef KOMPILATOR_SRC_COMPILER_H_
#define KOMPILATOR_SRC_COMPILER_H_



class Compiler {


 public:

  explicit Compiler();
  void set_output(_IO_FILE* output_file);

  void add_variable(const std::string &name, variable_type type);
  variable* add_const_variable(literal value);
  variable* find_const(literal value);
  variable *get_const(unsigned long long int value);
  VALUE_BLOCK *get_block_const(unsigned long long int value);

  void handle_program();
  void handle_declaration(std::string name, variable_type type);
  VALUE_BLOCK* handle_variable(const std::string& name);
  VALUE_BLOCK* handle_literal(literal value);
  COMMAND_BLOCK* handle_write(VALUE_BLOCK* output);
  COMMAND_BLOCK* handle_read(std::string input_name);
  COMMAND_BLOCK* handle_if(CONDITION_BLOCK* condition_block, COMMANDS_BLOCK* commands_block);
  COMMANDS_BLOCK* handle_command(COMMANDS_BLOCK* commands_block, COMMAND_BLOCK* command);
  EXPRESSION_BLOCK* handle_expression(basic_blocks_types::expression_type expression_type, VALUE_BLOCK* lhs, VALUE_BLOCK* rhs);
  CONDITION_BLOCK* handle_condition(basic_blocks_types::condition_type condition_type, VALUE_BLOCK* lhs, VALUE_BLOCK* rhs);
  COMMAND_BLOCK* handle_assign(std::string identifier, EXPRESSION_BLOCK* expression_block);

  void handle_main(COMMANDS_BLOCK* commands_block);
  static std::string compiler_log(std::string msg, int log_level);
  static void set_log_level(int log_level);
  static int COMPILER_DEBUG_MODE;
 private:
  _IO_FILE* _output_file;
  std::map<std::string, variable> _variable_map;
  std::map<literal, variable> _const_map;
  unsigned int _memory_count;
  unsigned int _tag_count;
  MAIN_BLOCK* _main = nullptr;
  std::vector<std::string> main_code;

};

#endif //KOMPILATOR_SRC_COMPILER_H_
