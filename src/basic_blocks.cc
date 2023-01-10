//
// Created by krrer on 09.01.23.
//

#include "basic_blocks.h"

int GENERIC_BLOCK::get_type() {
  return _type;
}

std::vector<std::string> ADDITION_BLOCK::translate_block() {
  std::vector<std::string> code;
  if(_lhs->get_type() == NUM_BLOCK_TYPE){

      if(_rhs->get_type() == NUM_BLOCK_TYPE){

      } else{ // rhs is not literal
        code.push_back(_lhs->load_value());
        code.push_back("ADD " + _rhs->get_content());
      }
  } else{ // lhs is not literal
    code.push_back(_rhs->load_value());
    code.push_back("ADD " + _lhs->get_content());
  }
  return code;
}
ADDITION_BLOCK::ADDITION_BLOCK(VALUE_BLOCK *lhs, VALUE_BLOCK *rhs)
    : _rhs(rhs), _lhs(lhs) {
  _type = ADDITION_BLOCK_TYPE;
}

NUM_BLOCK::NUM_BLOCK(unsigned long long int value) :
    _number_literal(value) {
  _type = NUM_BLOCK_TYPE;
}
std::string NUM_BLOCK::get_content() const {
  return std::to_string(_number_literal);
}
std::string NUM_BLOCK::load_value() {
  return "SET " + std::to_string(_number_literal);
}



VARIABLE_BLOCK::VARIABLE_BLOCK(std::string name, variable_type variable_type):
  _var_name(name), _var_type(variable_type){
  _type = VARIABLE_BLOCK_TYPE;
}
std::string VARIABLE_BLOCK::load_value() {
  return "LOAD @" + _var_name;
}

//TODO change name to index
std::string VARIABLE_BLOCK::get_content() {
  return "@"+ std::string(_var_name);
}



std::vector<std::string> ASSIGN_BLOCK::translate_block() {
  std::vector<std::string> code;
  code = _rhs->translate_block();
  code.push_back("STORE " + _lhs->get_content());
  return code;
}
ASSIGN_BLOCK::ASSIGN_BLOCK(VARIABLE_BLOCK *lhs, EXPRESSION_BLOCK *rhs)
    : _rhs(rhs), _lhs(lhs) {
  _type = ASSIGN_BLOCK_TYPE;
}


PROGRAM_BLOCK::PROGRAM_BLOCK(VARIABLE_DECLARATION_BLOCK *declaration_block,
                             COMMANDS_BLOCK *commands_block) :
    _declaration_block(declaration_block), _commands_block(commands_block){
  _type = PROGRAM_BLOCK_TYPE;
}
