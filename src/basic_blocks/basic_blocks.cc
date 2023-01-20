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

std::vector<std::string> PROCEDURE_BLOCK::translate_block() {
  std::vector<std::string> code;
  code.push_back("&" + _proc_name);
  auto commands_code = _commands_block->translate_block();
  if(!commands_code.empty()) commands_code[0] +=  Compiler::compiler_log("[ " + _proc_name + " begin ]",4 );
  code.insert(code.end(), commands_code.begin(), commands_code.end());
  code.emplace_back("JUMPI " + std::to_string(_return_adress) + Compiler::compiler_log(" [ " + _proc_name + " end ]",4 ));
  return code;
}
PROCEDURE_BLOCK::PROCEDURE_BLOCK() = default;

PROCEDURE_CALL_BLOCK::PROCEDURE_CALL_BLOCK(std::vector<variable> *variables,
                                           std::string proc_name, PROCEDURE_BLOCK *_procedure_block)
    : _variables(variables), _procedure_block(_procedure_block) {
  _proc_name = proc_name;
}

std::vector<std::string> PROCEDURE_CALL_BLOCK::translate_block() {
  uint16_t times_used = _procedure_block->_no_uses++;
  std::vector<std::string> code;
  for (int i = 0; i < _variables->size(); i++) {
    auto proc_var = (*(_procedure_block->_variables))[i];
    auto var = (*_variables)[i];
    switch (var.type) {
      case value_var:code.push_back("SET " + std::to_string(var.memory_index) );
        code.push_back("STORE " + std::to_string(proc_var.memory_index));
        break;
      case pointer_var:code.push_back("LOAD  " + std::to_string(var.memory_index));
        code.push_back("STORE " + std::to_string(proc_var.memory_index));
        break;
      case const_var:code.push_back("SET " + std::to_string(var.memory_index));
        code.push_back("STORE " + std::to_string(proc_var.memory_index));
        break;
    }
  }
  code.push_back("SET &" + _proc_name + std::to_string(times_used) + "#return");
  code.push_back("STORE " + std::to_string(_procedure_block->_return_adress));
  code.push_back("JUMP  &" + _proc_name);
  code.push_back("&" + _proc_name + std::to_string(times_used) + "#return");
  return code;
}
