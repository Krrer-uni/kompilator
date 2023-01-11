//
// Created by krrer on 09.01.23.
//

#include <iostream>
#include "basic_blocks.h"

int GENERIC_BLOCK::get_type() {
  return _type;
}

//std::vector<std::string> ADDITION_BLOCK::translate_block() {
//  std::vector<std::string> code;
//  if (_lhs->get_type() == NUM_BLOCK_TYPE) {
//
//    if (_rhs->get_type() == NUM_BLOCK_TYPE) {
//
//    } else { // rhs is not literal
//      auto to_insert = _lhs->translate_block();
//      code.insert(code.end(), to_insert.begin(), to_insert.end());
//      code.push_back("ADD " + _rhs->get_content());
//    }
//  } else { // lhs is not literal
//    auto to_insert = _rhs->translate_block();
//    code.insert(code.end(), to_insert.begin(), to_insert.end());
//    code.push_back("ADD " + _lhs->get_content());
//  }
//  return code;
//}
//ADDITION_BLOCK::ADDITION_BLOCK(VALUE_BLOCK *lhs, VALUE_BLOCK *rhs)
//    : _rhs(rhs), _lhs(lhs) {
//  _type = ADDITION_BLOCK_TYPE;
//}
//
VALUE_BLOCK::VALUE_BLOCK(VALUE_BLOCK::value_type value_type) :
    _value_type(value_type),
    _var_block(nullptr),
    _num_block(nullptr){}

std::vector<std::string> VALUE_BLOCK::translate_block() {
  std::vector<std::string> code;

  switch (_value_type) {
    case var:code = _var_block->translate_block();
      break;
    case number:code = _num_block->translate_block();
      break;
    default:std::cout << "jeszcze tego nie zaimplementowałeś\n";
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
    _var_name(name), _var_type(variable_type), _memory_block(memory_index) {
}
std::vector<std::string> VAR_BLOCK::translate_block() {
  std::vector<std::string> code;
  code.push_back("LOAD @" + _var_name);
  return code;
}
std::string VAR_BLOCK::get_memory_adress() {
  std::string msg;
  if(_var_type == value_var){
    msg = " " + std::to_string(_memory_block);
  }else{
    msg = "I " + std::to_string(_memory_block);
  }
  return msg;
}

ASSIGN_BLOCK::ASSIGN_BLOCK(VAR_BLOCK *lhs, EXPRESSION_BLOCK *rhs)
    : _rhs(rhs), _lhs(lhs) {
}
std::vector<std::string> ASSIGN_BLOCK::translate_block() {
  return std::vector<std::string>();
}

WRITE_BLOCK::WRITE_BLOCK(VALUE_BLOCK *output) {
  _output = output;
}

std::vector<std::string> WRITE_BLOCK::translate_block() {
  std::vector<std::string> code;
  if (_output->_value_type == VALUE_BLOCK::var) {
    code.emplace_back("PUT" + _output->_var_block->get_memory_adress() + "  [ @"
                          + _output->_var_block->_var_name + " ]");
  } else {
    code.push_back("SET " + std::to_string(_output->_num_block->_number_literal));
    code.emplace_back("PUT 0");
  }

  return code;
}
COMMANDS_BLOCK::COMMANDS_BLOCK() {
  _type = COMMANDS_BLOCK_TYPE;
}

std::vector<std::string> COMMANDS_BLOCK::translate_block() {
  std::vector<std::string> code;
  std::vector<std::string> command_code;
  for (COMMAND_BLOCK *command : _commands) {
    command_code = command->translate_block();
    code.insert(code.end(), command_code.begin(), command_code.end());
  }
  return code;
}
void COMMANDS_BLOCK::add_command(COMMAND_BLOCK *command) {
  _commands.push_back(command);
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
COMMAND_BLOCK::COMMAND_BLOCK(command_type command_type)
    : _commnad_type(command_type),
      _write_block(nullptr),
      _assign_block(nullptr),
      _read_block(nullptr){

}

std::vector<std::string> COMMAND_BLOCK::translate_block() {
  std::vector<std::string> code;

  switch (_commnad_type) {
    case write:code = _write_block->translate_block();
      break;
    case assign:code = _assign_block->translate_block();
      break;
    case read:code = _read_block->translate_block();
      break;
    default:std::cout << "jeszcze nie zaimplementowałeś tej komendy \n";
      break;
  }

  return code;
}

READ_BLOCK::READ_BLOCK(VALUE_BLOCK *input) {
  _input = input;
}

std::vector<std::string> READ_BLOCK::translate_block() {
  std::vector<std::string> code;
    code.emplace_back("GET " + std::to_string(_input->_var_block->_memory_block) + "  [ @"
                          + _input->_var_block->_var_name + " ]");
  return code;
}
EXPRESSION_BLOCK::EXPRESSION_BLOCK(EXPRESSION_BLOCK::expression_type type,
                                   VALUE_BLOCK *lhs,
                                   VALUE_BLOCK *rhs) :
                                   _type(type), _lhs(lhs), _rhs(rhs){

}
std::vector<std::string> EXPRESSION_BLOCK::translate_block() {
  switch (_type) {
    case add:

  }
}
