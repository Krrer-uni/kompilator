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
}

void Compiler::handle_program() {
  main_code = _main->translate_block();
  for (const auto &line : main_code) {
    std::cout << line << std::endl;
  }
}

void Compiler::add_variable(const std::string &name, variable_type type) {
  _variable_map[name] = variable{name, type, _memory_count};
  _memory_count++;
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
  auto value_block = new VALUE_BLOCK(VALUE_BLOCK::var);
  value_block->_var_block = var_block;
  if (_debug_mode > 0) {
    std::cout << "added block for var " + name + "\n";
  }
  return value_block;
}
VALUE_BLOCK *Compiler::handle_literal(unsigned long long int value) {
  auto num_block = new NUM_BLOCK(value);
  auto value_block = new VALUE_BLOCK(VALUE_BLOCK::number);
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
  auto command_block = new COMMAND_BLOCK(COMMAND_BLOCK::write);
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
  auto value_block = new VALUE_BLOCK(VALUE_BLOCK::var);
  value_block->_var_block = var_block;
  auto read_block = new READ_BLOCK(value_block);
  auto command_block = new COMMAND_BLOCK(COMMAND_BLOCK::read);
  command_block->_read_block = read_block;

  if (_debug_mode > 0) {
    std::cout << "added READ block \n";
  }
  return command_block;
}


