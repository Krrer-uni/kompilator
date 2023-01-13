//
// Created by krrer on 09.01.23.
//

#include <iostream>
#include <utility>
#include "basic_blocks.h"
#include "../compiler.h"

int GENERIC_BLOCK::get_type() {
  return _type;
}

VALUE_BLOCK::VALUE_BLOCK(basic_blocks_types::value_type value_type) :
    _value_type(value_type),
    _var_block(nullptr),
    _num_block(nullptr) {}

std::string VALUE_BLOCK::to_acc() {
  std::string code;
  switch (_value_type) {
    case basic_blocks_types::VAL_VAR:code = "LOAD" + _var_block->get_memory_adress();
      break;
    case basic_blocks_types::VAL_LIT:code = "SET " + std::to_string(_num_block->_number_literal);
      break;
  }
  return code;
}

NUM_BLOCK::NUM_BLOCK(unsigned long long int value) :
    _number_literal(value) {
}
std::vector<std::string> NUM_BLOCK::translate_block() {
  std::vector<std::string> code;
  code.push_back("SET " + std::to_string(_number_literal));
  return code;
}

VAR_BLOCK::VAR_BLOCK(std::string name, variable_type variable_type, unsigned int memory_index) :
    _var_name(std::move(name)), _var_type(variable_type), _memory_block(memory_index) {
}

std::string VAR_BLOCK::get_memory_adress() {
  std::string msg;

  if (_var_type == pointer_var) {
    msg = "I " + std::to_string(_memory_block)
        + Compiler::compiler_log("   [ @" + _var_name + " ]", 5);
  } else {
    msg = " " + std::to_string(_memory_block)
        + Compiler::compiler_log("   [ @" + _var_name + " ]", 5);
  }
  return msg;
}

MAIN_BLOCK::MAIN_BLOCK(COMMANDS_BLOCK *commands_block) {
  _commands_block = commands_block;
  _type = MAIN_BLOCK_TYPE;
}
std::vector<std::string> MAIN_BLOCK::translate_block() {
  std::vector<std::string> code;
  code = _commands_block->translate_block();
  code.emplace_back("HALT");
  return code;
}


