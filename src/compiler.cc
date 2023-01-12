//
// Created by krrer on 09.01.23.
//
#include <map>
#include <iostream>
#include "compiler.h"

Compiler::Compiler(int debug_mode) {
  _variable_map = std::map<std::string, variable>();
  _debug_mode = debug_mode;
  _memory_count = 1;
  _output_file = nullptr;
}

void Compiler::handle_program() {
  main_code = _main->translate_block();
  if(_output_file == nullptr){
    std::cout << "NO OUTPUT FILE FOUND \n";
    for (const auto &line : main_code) {
      std::cout << line << std::endl;
    }
  } else{
    for (const auto &line : main_code) {
      fprintf(_output_file, "%s", (line + "\n").c_str());
    }
  }

}

void Compiler::add_variable(const std::string &name, variable_type type) {
  if(_variable_map.find(name) == _variable_map.end()) {
    _variable_map[name] = variable{name, type, _memory_count};
    _memory_count++;
  }else{
    std::cout << "Variable with that name already exist";
  }
}


void Compiler::handle_declaration(std::string name, variable_type type) {
  add_variable(name, type);
  if (_debug_mode > 0) {
    std::cout << "created var " + name + " with type " + std::to_string(type) + "\n";
  }
}
VALUE_BLOCK *Compiler::handle_variable(const std::string &name) {
  if (_variable_map.find(name) == _variable_map.end()) {
    std::cout << "variable " + name + " was not declared in the scope";
    return nullptr;
  }
  auto var = _variable_map[name];
  auto var_block = new VAR_BLOCK(name, var.type, var.memory_index);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_VAR);
  value_block->_var_block = var_block;
  if (_debug_mode > 0) {
    std::cout << "added block for var " + name + "\n";
  }
  return value_block;
}
VALUE_BLOCK *Compiler::handle_literal(unsigned long long int value) {
  auto num_block = new NUM_BLOCK(value);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_LIT);
  value_block->_num_block = num_block;
  if (_debug_mode > 0) {
    std::cout << "added block for literal " + std::to_string(value) + "\n";
  }
  return value_block;
}
COMMAND_BLOCK *Compiler::handle_write(VALUE_BLOCK *output) {
  if (_debug_mode > 0) {
    std::cout << "added WRITE block \n";
  }
  auto write_block = new WRITE_BLOCK(output);
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_WRITE);
  command_block->_write_block = write_block;
  return command_block;
}

COMMANDS_BLOCK *Compiler::handle_command(COMMANDS_BLOCK *commands_block, COMMAND_BLOCK *command) {
  if (_debug_mode > 0) {
    std::cout << "added COMMAND block to COMMANDS \n";
  }
  if (commands_block == nullptr) {
    commands_block = new COMMANDS_BLOCK();
  }
  commands_block->add_command(command);
  return commands_block;
}
void Compiler::handle_main(COMMANDS_BLOCK *commands_block) {
  if (_debug_mode > 0) {
    std::cout << "created MAIN block \n";
  }

  _main = new MAIN_BLOCK(commands_block);
}

COMMAND_BLOCK *Compiler::handle_read(std::string input_name) {
  if (_variable_map.find(input_name) == _variable_map.end()) {
    std::cout << "variable " + input_name + " was not declared in the scope";
    return nullptr;
  }
  auto var = _variable_map[input_name];
  auto var_block = new VAR_BLOCK(input_name, var.type, var.memory_index);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_VAR);
  value_block->_var_block = var_block;
  auto read_block = new READ_BLOCK(value_block);
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_READ);
  command_block->_read_block = read_block;

  if (_debug_mode > 0) {
    std::cout << "added READ block \n";
  }
  return command_block;
}
variable* Compiler::add_const_variable(const unsigned long long int value) {
  std::string name = std::to_string(value);
  variable* new_var;
  if(_const_map.find(value) == _const_map.end()) {
    new_var  = new variable{name, const_var, _memory_count};
    _const_map[value] = *new_var;
    _memory_count++;
  }else{
    std::cout << "Variable with that name already exist";
    new_var = nullptr;
  }
  return new_var;
}

variable *Compiler::find_const(unsigned long long int value) {
  variable* map_val;
  if(_const_map.find(value) == _const_map.end()) {
    map_val =  nullptr;
  } else{
    map_val =  &_const_map[value];
  }
  return map_val;
}

EXPRESSION_BLOCK *Compiler::handle_expression(basic_blocks_types::expression_type expression_type,
                                              VALUE_BLOCK *lhs,
                                              VALUE_BLOCK *rhs) {
  auto block = new EXPRESSION_BLOCK(expression_type,lhs,rhs,this);
  if (_debug_mode > 0) {
    std::cout << "added EXPRESSION block \n";
  }
  return block;
}
COMMAND_BLOCK *Compiler::handle_assign(std::string identifier, EXPRESSION_BLOCK *expression_block) {
  if (_variable_map.find(identifier) == _variable_map.end()) {
    std::cout << "variable " + identifier + " was not declared in the scope";
    return nullptr;
  }
  auto var = _variable_map[identifier];
  auto var_block = new VAR_BLOCK(identifier, var.type, var.memory_index);
  auto cmd_block = new COMMAND_BLOCK(basic_blocks_types::CMD_ASIGN);
  cmd_block->_assign_block = new ASSIGN_BLOCK(var_block,expression_block);
  if (_debug_mode > 0) {
    std::cout << "added ASSIGN block \n";
  }
  return cmd_block;
}
void Compiler::set_output(_IO_FILE *output_file) {
  _output_file = output_file;
}


